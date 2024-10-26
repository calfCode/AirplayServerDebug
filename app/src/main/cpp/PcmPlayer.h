//
// Created by Lenovo on 2023/7/31.
//

#ifndef AIRPLAYSERVER_PCMPLAYER_H
#define AIRPLAYSERVER_PCMPLAYER_H
//#include <oboe/Oboe.h>
#include <math.h>
#include <queue>
#include "stream.h"
#include "log.h"
//using namespace oboe;

class PcmPlayer{
public:
//    oboe::DataCallbackResult onAudioReady(oboe::AudioStream *oboeStream, void *audioData, int32_t numFrames) override {
////        LOGD("BRUCE onAudioReady 0");
//        if (!audioQueque.empty()){
////            LOGD("BRUCE onAudioReady 1");
//            pcm_data_struct data = audioQueque.front();
//            memcpy(audioData,data.data,data.data_len);
//            audioQueque.pop();
//        }
//        return oboe::DataCallbackResult::Continue;
//    }

    virtual ~PcmPlayer() = default;
    int32_t startAudio();
    void stopAudio() ;

    void addPacket(audio_decode_struct pcmDataStruct);

private:
//    std::mutex         mLock;
//    std::shared_ptr<oboe::AudioStream> mStream;
    // Stream params
    static int constexpr kChannelCount = 2;
    static int constexpr kSampleRate = 44100;

    std::queue<audio_decode_struct> audioQueque;
};


#endif //AIRPLAYSERVER_PCMPLAYER_H
