//
// Created by Lenovo on 2023/3/13.
//

#include <__threading_support>
#include <unistd.h>
#include "H264Decoder.h"
#include "log.h"

H264Decoder::H264Decoder() {

}

H264Decoder::~H264Decoder() {

}

void H264Decoder::addPacket(h264_data_struct data) {
    LOGD("addPacket");
    videoQueque.push(data);
}

static void *input(void *privdata) {
    auto *decoder = (H264Decoder *) privdata;
    decoder->inputThread();
    return nullptr;
}

static void *output(void *privdata) {
    auto *decoder = (H264Decoder *) privdata;
    decoder->outputThread();
    return nullptr;
}

int H264Decoder::start(ANativeWindow *wind,h264_decoder_callbacks_t *callbacks) {
    LOGD("start");
    bRun = true;
    memcpy(&this->callbacks,callbacks, sizeof(h264_decoder_callbacks_t));
    pMediaCodec = AMediaCodec_createDecoderByType("video/avc");//h264
    AMediaFormat *format = AMediaFormat_new();
    AMediaFormat_setString(format, "mime", "video/avc");
    AMediaFormat_setInt32(format, AMEDIAFORMAT_KEY_WIDTH, 1920);
    AMediaFormat_setInt32(format, AMEDIAFORMAT_KEY_HEIGHT, 1080);
    media_status_t status = AMediaCodec_configure(pMediaCodec, format, wind, nullptr, 0);
    AMediaFormat_delete(format);
    if (status != 0) {
        LOGD("erro config %d", status);
        return -1;
    }
    AMediaCodec_start(pMediaCodec);

    //也可以通过 AMediaCodec_setAsyncNotifyCallback 设置成异步方式.
    if (pthread_create(&mThreadIn, nullptr, input, (void *) this) != 0) {
        LOGE("[%s%d] pthread create err!!", __FUNCTION__, __LINE__);
        return -1;
    }
    if (pthread_create(&mThreadOut, nullptr, output, (void *) this) != 0) {
        LOGE("[%s%d] pthread create err!!", __FUNCTION__, __LINE__);
        return -1;
    }

    return 0;
}

void H264Decoder::stop() {
    bRun = false;
    if (pMediaCodec != nullptr) {
        if (mThreadIn != -1) {
            pthread_join(mThreadIn, nullptr);
            mThreadIn = -1;
        }
        if (mThreadOut != -1) {
            pthread_join(mThreadOut, nullptr);
            mThreadOut = -1;
        }
        AMediaCodec_stop(pMediaCodec);
    }
}

void H264Decoder::inputThread() {
    LOGD("inputThread");
    while (bRun) {
        if (videoQueque.empty()) {
            // 避免空转，还是休眠一下
            usleep(1000);
            continue;
        }
        h264_data_struct buffer = videoQueque.front();
        videoQueque.pop();
        ssize_t bufidx = 0;
        do {
            bufidx = AMediaCodec_dequeueInputBuffer(pMediaCodec, 1000 * 20);
            if (bufidx >= 0) {
                size_t bufsize;
                uint8_t *buf = AMediaCodec_getInputBuffer(pMediaCodec, bufidx, &bufsize);
//                ALOGD("[%s%d] AMediaCodec_queueInputBuffer one", __FUNCTION__, __LINE__);
                memcpy(buf, buffer.data, buffer.data_len);
                AMediaCodec_queueInputBuffer(pMediaCodec, bufidx, 0, buffer.data_len, buffer.pts,
                                             0);
            }
        } while (bufidx < 0);
    }
}

void H264Decoder::outputThread() {
    LOGD("outputThread");
    while (bRun) {
        AMediaCodecBufferInfo info;
        ssize_t bufidx = 0;
        bufidx = AMediaCodec_dequeueOutputBuffer(pMediaCodec, &info, 1000 * 20);
        if (bufidx >= 0) {
            //uint8_t *buf buf = AMediaCodec_getOutputBuffer(pMediaCodec, bufidx, &bufsize);
            AMediaCodec_releaseOutputBuffer(pMediaCodec, bufidx, true);
        } else if (bufidx == AMEDIACODEC_INFO_OUTPUT_FORMAT_CHANGED) {
            int w = 0, h = 0;
            auto format = AMediaCodec_getOutputFormat(pMediaCodec);
            AMediaFormat_getInt32(format, "width", &w);
            AMediaFormat_getInt32(format, "height", &h);

            float fps = 0;
//            AMediaFormat_getFloat(format,AMEDIAFORMAT_KEY_MAX_FPS_TO_ENCODER,&fps);
            int32_t localColorFMT;
            AMediaFormat_getInt32(format, AMEDIAFORMAT_KEY_COLOR_FORMAT,
                                  &localColorFMT);
            LOGD("[%s%d] AMEDIACODEC_INFO_OUTPUT_FORMAT_CHANGED:%dx%d,color:%d fps:%f",
                 __FUNCTION__,
                 __LINE__, w, h, localColorFMT, fps);

            callbacks.videoSizeChange(w,h);

        } else if (bufidx == AMEDIACODEC_INFO_OUTPUT_BUFFERS_CHANGED) {
        } else {
        }
    }
}

