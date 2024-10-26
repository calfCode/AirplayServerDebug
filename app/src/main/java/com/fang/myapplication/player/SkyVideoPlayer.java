package com.fang.myapplication.player;

import static android.media.MediaCodec.VIDEO_SCALING_MODE_SCALE_TO_FIT;

import android.media.MediaCodec;
import android.media.MediaFormat;
import android.util.Log;
import android.view.Surface;

import com.fang.myapplication.model.NALPacket;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class SkyVideoPlayer extends Thread {
    private static final String TAG = "SkyVideoPlayer";
    private Surface surface;
    private MediaCodec mediaCodec;
    private String mime = "video/avc";
    private int width = 1920;
    private int height = 1080;
    private List<NALPacket> videoBuffer = Collections.synchronizedList(new ArrayList<NALPacket>());
    MediaCodec.BufferInfo bufferInfo = new MediaCodec.BufferInfo();
    private boolean isRunning = true;
    public SkyVideoPlayer(Surface surface,int width,int height){
        this.surface = surface;
        this.width = width;
        this.height = height;

    }

    private void initDecoder(){
        try {
            MediaFormat format = MediaFormat.createVideoFormat(mime, width, height);
            mediaCodec = MediaCodec.createDecoderByType(mime);
            mediaCodec.configure(format, surface, null, 0);
            mediaCodec.setVideoScalingMode(MediaCodec.VIDEO_SCALING_MODE_SCALE_TO_FIT);
            mediaCodec.start();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void run() {
        super.run();
        initDecoder();

        while (isRunning) {
            if (videoBuffer.size() <= 0) {
                try {
                    sleep(50);

                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                continue;
            }
            decodePacket(videoBuffer.remove(0));
        }
    }

    public void decodePacket(NALPacket packet){
        Log.d(TAG,"decodePacket 2222222222222222222");
        if (mediaCodec!=null){
            final int TIMEOUT_USEC = 10000;
            ByteBuffer[] decoderInputBuffers = mediaCodec.getInputBuffers();
            int inputBufferIndex = -1;
            try {
                inputBufferIndex=mediaCodec.dequeueInputBuffer(TIMEOUT_USEC);
            }catch (Exception e){
                e.printStackTrace();
            }

            if (inputBufferIndex>=0){
//                ByteBuffer inputBuffer = decoderInputBuffers[inputBufferIndex];
                ByteBuffer inputBuffer =  mediaCodec.getInputBuffer(inputBufferIndex);
                inputBuffer.put(packet.nalData);
                mediaCodec.queueInputBuffer(inputBufferIndex,0,packet.nalData.length,packet.pts,0);
            }else {
                Log.d(TAG,"dequeueInputBuffer fail");
            }

            int outputBufferIndex = -1;
            try {
                outputBufferIndex = mediaCodec.dequeueOutputBuffer(bufferInfo,TIMEOUT_USEC);
            }catch (Exception e){
                e.printStackTrace();
            }
            Log.d(TAG,"decodePacket outputBufferIndex="+outputBufferIndex);
            if (outputBufferIndex>=0){
                mediaCodec.releaseOutputBuffer(outputBufferIndex,true);
                try {
                    sleep(10);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }else if (outputBufferIndex == MediaCodec.INFO_TRY_AGAIN_LATER) {
                try{
                    Thread.sleep(10);
                }  catch (InterruptedException ie){
                    ie.printStackTrace();
                }
            } else if (outputBufferIndex == MediaCodec.INFO_OUTPUT_BUFFERS_CHANGED) {
                // not important for us, since we're using Surface

            } else if (outputBufferIndex == MediaCodec.INFO_OUTPUT_FORMAT_CHANGED) {

            } else {

            }
        }
    }

    public void addPacker(NALPacket nalPacket){
        videoBuffer.add(nalPacket);
    }


}
