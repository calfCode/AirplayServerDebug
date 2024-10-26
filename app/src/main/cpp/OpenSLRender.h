//
// Created by Lenovo on 2022/12/20.
//

#ifndef MYFFMPEG_OPENSLRENDER_H
#define MYFFMPEG_OPENSLRENDER_H


#include <cstdint>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <queue>
#include <string>
#include <thread>


#define MAX_QUEUE_BUFFER_SIZE 3
class AudioFrame {
public:
    AudioFrame(unsigned short * data, int dataSize, bool hardCopy = true) {
        this->dataSize = dataSize;
        this->data = data;
        this->hardCopy = hardCopy;
        if(hardCopy) {
            this->data = static_cast<unsigned short *>(malloc(this->dataSize* sizeof(unsigned short)));
            memcpy(this->data, data, dataSize);
        }
    }

    ~AudioFrame() {
        if(hardCopy && this->data)
            free(this->data);
        this->data = nullptr;
    }

    unsigned short * data = nullptr;
    int dataSize = 0;
    bool hardCopy = true;
};
class OpenSLRender  {
public:
    OpenSLRender(){}
    virtual ~OpenSLRender(){}
    virtual void Init();
    virtual void ClearAudioCache();
    virtual void RenderAudioFrame(unsigned short *pData, int dataSize);
    virtual void UnInit();

private:
    int CreateEngine();
    int CreateOutputMixer();
    int CreateAudioPlayer();
    int GetAudioFrameQueueSize();
    void StartRender();
    void HandleAudioFrameQueue();
    static void CreateSLWaitingThread(OpenSLRender *openSlRender);
    static void AudioPlayerCallback(SLAndroidSimpleBufferQueueItf bufferQueue, void *context);

    SLObjectItf m_EngineObj = nullptr;
    SLEngineItf m_EngineEngine = nullptr;
    SLObjectItf m_OutputMixObj = nullptr;
    SLObjectItf m_AudioPlayerObj = nullptr;
    SLPlayItf m_AudioPlayerPlay = nullptr;
    SLVolumeItf m_AudioPlayerVolume = nullptr;
    SLAndroidSimpleBufferQueueItf m_BufferQueue;

    std::queue<AudioFrame *> m_AudioFrameQueue;

    std::thread *m_thread = nullptr;
    std::mutex   m_Mutex;
    std::condition_variable m_Cond;
    volatile bool m_Exit = false;
};




#endif //MYFFMPEG_OPENSLRENDER_H
