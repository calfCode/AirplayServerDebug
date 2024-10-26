//
// Created by Lenovo on 2023/7/31.
//

#include "PcmPlayer.h"
int32_t PcmPlayer::startAudio(){
//    std::lock_guard<std::mutex> lock(mLock);
//    oboe::AudioStreamBuilder builder;
//    Result result = builder.setSharingMode(oboe::SharingMode::Exclusive)
//            ->setPerformanceMode(oboe::PerformanceMode::LowLatency)
//            ->setChannelCount(kChannelCount)
//            ->setSampleRate(kSampleRate)
//            ->setSampleRateConversionQuality(oboe::SampleRateConversionQuality::Best)
//            ->setFormat(oboe::AudioFormat::I16)
//            ->setDataCallback(this)
//            ->openStream(mStream);
//    if (result != Result::OK) return (int32_t) result;
//
//    // Typically, start the stream after querying some stream information, as well as some input from the user
//    result = mStream->requestStart();
//    return (int32_t) result;
return 0;
}

void PcmPlayer::stopAudio() {
//    std::lock_guard<std::mutex> lock(mLock);
//    if (mStream) {
//        mStream->stop();
//        mStream->close();
//        mStream.reset();
//    }
}

void PcmPlayer::addPacket(audio_decode_struct pcmDataStruct) {
//    audioQueque.push(pcmDataStruct);
}