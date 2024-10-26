package com.fang.myapplication;

import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import com.fang.myapplication.model.NALPacket;
import com.fang.myapplication.model.PCMPacket;
import com.fang.myapplication.player.AudioPlayer;
import com.fang.myapplication.player.SkyVideoPlayer;
import com.fang.myapplication.player.VideoPlayer;

public class RaopServer implements SurfaceHolder.Callback {

    static {
        System.loadLibrary("raop_server");
        System.loadLibrary("play-lib");
    }
    private static final String TAG = "RaopServer";
    private VideoPlayer mVideoPlayer;
    private AudioPlayer mAudioPlayer;
    private SurfaceView mSurfaceView;
    private long mServerId = 0;
    private SkyVideoPlayer skyVideoPlayer;

    public RaopServer(SurfaceView surfaceView) {
        mSurfaceView = surfaceView;
        mSurfaceView.getHolder().addCallback(this);
        mAudioPlayer = new AudioPlayer();
        mAudioPlayer.start();
    }

    public void onRecvVideoData(byte[] nal, int nalType, long dts, long pts) {
        Log.d(TAG, "onRecvVideoData pts = " + pts + ", nalType = " + nalType + ", nal length = " + nal.length);
        NALPacket nalPacket = new NALPacket();
        nalPacket.nalData = nal;
        nalPacket.nalType = nalType;
        nalPacket.pts = pts;
        if (mVideoPlayer!=null) {
            mVideoPlayer.addPacker(nalPacket);
        }
        if (skyVideoPlayer!=null){
            skyVideoPlayer.addPacker(nalPacket);
        }
    }

    public void onRecvAudioData(short[] pcm, long pts) {
        Log.d(TAG, "onRecvAudioData pcm length = " + pcm.length + ", pts = " + pts);
        if (mAudioPlayer!=null) {
            PCMPacket pcmPacket = new PCMPacket();
            pcmPacket.data = pcm;
            pcmPacket.pts = pts;

            mAudioPlayer.addPacker(pcmPacket);
        }

    }
    public static byte[] short2byte(short s){
        byte[] b = new byte[2];
        for(int i = 0; i < 2; i++){
            int offset = 16 - (i+1)*8; //因为byte占4个字节，所以要计算偏移量
            b[i] = (byte)((s >> offset)&0xff); //把16位分为2个8位进行分别存储
        }
        return b;
    }
    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        init_hwvideoPlay(holder.getSurface());
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
//        if (mVideoPlayer == null) {
//            mVideoPlayer = new VideoPlayer(holder.getSurface());
//            mVideoPlayer.start();
//        }
//        Log.d(TAG,"surfaceChanged width="+width+";height="+height);


//        if (skyVideoPlayer == null){
//            skyVideoPlayer = new SkyVideoPlayer(holder.getSurface(),width,height);
//            skyVideoPlayer.start();
//        }
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }

    public void startServer() {
        if (mServerId == 0) {
            mServerId = start();
        }
    }

    public void stopServer() {
        if (mServerId != 0) {
            stop(mServerId);
        }
        mServerId = 0;

    }

    public int getPort() {
        if (mServerId != 0) {
            return getPort(mServerId);
        }
        return 0;
    }

    public int getAirplayPort() {
        if (mServerId != 0) {
            return getAirplayPort(mServerId);
        }
        return 0;
    }

    private native long start();
    private native void stop(long serverId);
    private native int getPort(long serverId);
    public native void  init_hwvideoPlay(Surface surface);
    private static VideoListener videoListener;
    private native int getAirplayPort(long serverId);
    public static void setVideoListener(VideoListener listener){
        videoListener = listener;
    }
    public static interface VideoListener{
        public  void videoSize(int width,int height);
    }

    public static void videoSizeChange(int width,int height){
        if (videoListener!=null){
            videoListener.videoSize(width, height);
        }
    }

}
