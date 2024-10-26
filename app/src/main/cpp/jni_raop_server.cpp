//
// Created by Administrator on 2019/1/29/029.
//

#include <jni.h>
#include <stddef.h>
#include "lib/raop.h"
#include "log.h"
#include "lib/stream.h"
#include "lib/logger.h"
#include <malloc.h>
#include <cstring>
#include <thread>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "j4a_allclasses.h"
#ifdef __cplusplus
}
#endif
static JavaVM *g_JavaVM;

#include "H264Decoder.h"
#include "OpenSLRender.h"
#include "PcmPlayer.h"

static bool isDebug = true;
H264Decoder *h264decoder;
static bool isUseOpenSL = false;
OpenSLRender *openSLRender;
PcmPlayer *pcmPlayer;
static bool isUseOboe = false;
void OnRecvAudioData(void *observer, audio_decode_struct *data) {
    if (isUseOpenSL) {
        if (openSLRender!= nullptr){
            unsigned short *buffer =(unsigned short *) malloc(data->data_len* sizeof(unsigned short));
            memcpy(buffer,data->data,data->data_len);
            int lenght = data->data_len;
            openSLRender->RenderAudioFrame(buffer,lenght);
        }
    }else if(isUseOboe){
        if (pcmPlayer!=NULL){
            pcmPlayer->addPacket(*data);
        }
    }
    else {

        jobject obj = (jobject) observer;
        JNIEnv *jniEnv = NULL;
        g_JavaVM->AttachCurrentThread(&jniEnv, NULL);
        jclass cls = jniEnv->GetObjectClass(obj);
        jmethodID onRecvVideoDataM = jniEnv->GetMethodID(cls, "onRecvAudioData", "([SJ)V");
        jniEnv->DeleteLocalRef(cls);
        jshortArray sarr = jniEnv->NewShortArray(data->data_len);
        if (sarr == NULL) return;
        jniEnv->SetShortArrayRegion(sarr, (jint) 0, data->data_len, (jshort *) data->data);
        jniEnv->CallVoidMethod(obj, onRecvVideoDataM, sarr, data->ntp_time_local);
        jniEnv->DeleteLocalRef(sarr);
        g_JavaVM->DetachCurrentThread();
    }

}


void OnRecvVideoData(void *observer, h264_decode_struct *data) {
    if (isDebug) {
        if (h264decoder != nullptr) {
            h264_data_struct *h264_data = (h264_data_struct *) malloc(sizeof(h264_data_struct));
            if (h264_data != nullptr) {
                h264_data->data = (unsigned char *) malloc(data->data_len);
                h264_data->data_len = data->data_len;
                h264_data->pts = data->ntp_time_local;
                LOGD("OnRecvVideoData h264_data->pts=%ld", h264_data->pts);
                memcpy(h264_data->data, data->data, data->data_len);
                h264decoder->addPacket(*h264_data);
            }

        }
    } else {
        jobject obj = (jobject) observer;
        JNIEnv *jniEnv = NULL;
        g_JavaVM->AttachCurrentThread(&jniEnv, NULL);
        jclass cls = jniEnv->GetObjectClass(obj);
        jmethodID onRecvVideoDataM = jniEnv->GetMethodID(cls, "onRecvVideoData", "([BIJJ)V");
        jniEnv->DeleteLocalRef(cls);
        jbyteArray barr = jniEnv->NewByteArray(data->data_len);
        if (barr == NULL) return;
        jniEnv->SetByteArrayRegion(barr, (jint) 0, data->data_len, (jbyte *) data->data);
        jniEnv->CallVoidMethod(obj, onRecvVideoDataM, barr, 0,
                               data->ntp_time_local, data->ntp_time_local);
        jniEnv->DeleteLocalRef(barr);
        g_JavaVM->DetachCurrentThread();
    }


}

extern "C" void
audio_process(void *cls, raop_ntp_t *ntp, audio_decode_struct *data) {
    OnRecvAudioData(cls, data);
}

extern "C" void
audio_set_volume(void *cls, float volume) {

}

extern "C" void
video_process(void *cls,  raop_ntp_t *ntp,h264_decode_struct *data) {
    OnRecvVideoData(cls, data);
}

extern "C" void
log_callback(void *cls, int level, const char *msg) {
    switch (level) {
        case LOGGER_DEBUG: {
            LOGD("%s", msg);
            break;
        }
        case LOGGER_WARNING: {
            LOGW("%s", msg);
            break;
        }
        case LOGGER_INFO: {
            LOGI("%s", msg);
            break;
        }
        case LOGGER_ERR: {
            LOGE("%s", msg);
            break;
        }
        default:
            break;
    }

}
static pthread_key_t g_thread_key;
static pthread_once_t g_key_once = PTHREAD_ONCE_INIT;

static void ThreadDestroyed(void* value)
{
    JNIEnv *env = (JNIEnv*) value;
    if (env != NULL) {
        ALOGE("%s: [%d] didn't call SDL_JNI_DetachThreadEnv() explicity\n", __func__, (int)gettid());
        g_JavaVM->DetachCurrentThread();
        pthread_setspecific(g_thread_key, NULL);
    }
}
static void make_thread_key()
{
    pthread_key_create(&g_thread_key, ThreadDestroyed);
}
jint SetupThreadEnv(JNIEnv **p_env)
{
    JavaVM *jvm = g_JavaVM;
    if (!jvm) {
        ALOGE("SDL_JNI_GetJvm: AttachCurrentThread: NULL jvm");
        return -1;
    }

    pthread_once(&g_key_once, make_thread_key);

    JNIEnv *env = (JNIEnv*) pthread_getspecific(g_thread_key);
    if (env) {
        *p_env = env;
        return 0;
    }

    if (jvm->AttachCurrentThread(&env, NULL) == JNI_OK) {
        pthread_setspecific(g_thread_key, env);
        *p_env = env;
        return 0;
    }

    return -1;
}
extern "C" JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    g_JavaVM = vm;
    JNIEnv* env = NULL;
    if (vm->GetEnv((void**)&env,JNI_VERSION_1_4)!=JNI_OK){
        return -1;
    }
    assert(env!=NULL);
    int retval;
    retval = J4A_LoadAll__catchAll(env);
    return JNI_VERSION_1_6;
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_fang_myapplication_RaopServer_start(JNIEnv *env, jobject object) {
    raop_t *raop;
    raop_callbacks_t raop_cbs;
    memset(&raop_cbs, 0, sizeof(raop_cbs));
    raop_cbs.cls = (void *) env->NewGlobalRef(object);;
    raop_cbs.audio_process = audio_process;
    raop_cbs.audio_set_volume = audio_set_volume;
    raop_cbs.video_process = video_process;
    raop = raop_init(&raop_cbs);
    if (raop == NULL) {
        LOGE("raop = NULL");
        return 0;
    } else {
        LOGD("raop init success");
    }

    raop_set_log_callback(raop, log_callback, NULL);
//    raop_set_log_level(raop, RAOP_LOG_DEBUG);

    unsigned short port = 0;
    unsigned short airplayPort =0;
    raop_start(raop, &port,&airplayPort);
    raop_set_port(raop, port,airplayPort);
    LOGD("raop port = % d", raop_get_port(raop));
    return (jlong) (void *) raop;
}

extern "C" JNIEXPORT jint JNICALL
Java_com_fang_myapplication_RaopServer_getPort(JNIEnv *env, jobject object, jlong opaque) {
    raop_t *raop = (raop_t *) (void *) opaque;
    return raop_get_port(raop);
}

extern "C" JNIEXPORT jint JNICALL
Java_com_fang_myapplication_RaopServer_getAirplayPort(JNIEnv *env, jobject object, jlong opaque) {
    raop_t *raop = (raop_t *) (void *) opaque;
    return raop_get_airplay_port(raop);
}

extern "C" JNIEXPORT void JNICALL
Java_com_fang_myapplication_RaopServer_stop(JNIEnv *env, jobject object, jlong opaque) {
    raop_t *raop = (raop_t *) (void *) opaque;
    jobject obj = (jobject) raop_get_callback_cls(raop);
    raop_destroy(raop);
    env->DeleteGlobalRef(obj);

}

extern "C" void
videoSizeChange(int width, int height) {
    JNIEnv* env = NULL;
    LOGD("JNI videoSizeChange width=%d,height=%d",width,height);

    if (JNI_OK != SetupThreadEnv(&env)) {
        LOGD("JNI videoSizeChange SetupThreadEnv FAIL");
        return ;
    }
    J4AC_RaopServer__videoSizeChange(env,width,height);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_fang_myapplication_RaopServer_init_1hwvideoPlay(JNIEnv *env, jobject thiz,
                                                         jobject surface) {

    if (isDebug) {
        h264decoder = new H264Decoder();
        H264Decoder::h264_decoder_callbacks_t callbacks;
        memset(&callbacks, 0, sizeof(callbacks));
        callbacks.videoSizeChange = videoSizeChange;
        h264decoder->start(ANativeWindow_fromSurface(env, surface),&callbacks);
    }
    pcmPlayer = new PcmPlayer();
    pcmPlayer->startAudio();

//    int shortLength = sizeof(unsigned short );
//    LOGD("hwvideoPlay shortLength=%d",shortLength);
//    int uint8Length = sizeof(uint8_t);
//    LOGD("hwvideoPlay uint8Length=%d",uint8Length);
//    unsigned short testS[] ={0x0102,0x0304,0x0506,0x0708};
//    uint8_t*  test8 = (uint8_t*)malloc(4*2);
//    for (int i = 0; i < 4; ++i) {
//        unsigned short value = testS[i];
//        LOGD("hwvideoPlay value[%d]=%04x",i,value);
//        uint8_t value1 = value>>8 & 0xff;
//        LOGD("hwvideoPlay value1[%d]=%02x",i,value1);
//        uint8_t value2 = value & 0xff;
//        LOGD("hwvideoPlay value2[%d]=%02x",i,value2);
//        *test8 = value1;
//        *(test8+i) = value2;
//    }
//    for (int i = 0; i < 8; ++i) {
//        LOGD("hwvideoPlay test8[%d]=%02x",i,*(test8+i));
//    }
    if (isUseOpenSL){
        openSLRender = new OpenSLRender();
        openSLRender->Init();
    }

}

