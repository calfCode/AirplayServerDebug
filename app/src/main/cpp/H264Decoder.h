//
// Created by Lenovo on 2023/3/13.
//

#ifndef AIRPLAYSERVER_H264DECODER_H
#define AIRPLAYSERVER_H264DECODER_H


#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <media/NdkMediaCodec.h>
#include <media/NdkMediaExtractor.h>
#include <queue>
#include "H264Data.h"
using namespace std;
class H264Decoder {


public:
    struct h264_decoder_callbacks_s {
        void  (*videoSizeChange)(int width, int height);
    };
    typedef struct h264_decoder_callbacks_s h264_decoder_callbacks_t;
    H264Decoder();

    ~H264Decoder();

    int start(ANativeWindow *wind,h264_decoder_callbacks_t *callbacks);

    void stop() ;

    void inputThread();

    void outputThread();

    void addPacket(h264_data_struct data);
private:
    AMediaCodec *pMediaCodec;
    pthread_t mThreadIn;
    pthread_t mThreadOut;
    bool bRun;
    std::queue<h264_data_struct> videoQueque;
    h264_decoder_callbacks_t callbacks;
};


#endif //AIRPLAYSERVER_H264DECODER_H
