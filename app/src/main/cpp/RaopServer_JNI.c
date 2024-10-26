//
// Created by Lenovo on 2023/6/27.
//
#include "RaopServer_JNI.h"
#include "j4a/j4a_base.h"
#include "log.h"

typedef struct J4AC_com_fang_myapplication_RaopServer {
    jclass id;

    jfieldID field_mNativeMediaPlayer;
    jfieldID field_mNativeMediaDataSource;
    jfieldID field_mNativeAndroidIO;
    jmethodID method_videoSizeChange;
    jmethodID method_onSelectCodec;
    jmethodID method_onNativeInvoke;
} J4AC_com_fang_myapplication_RaopServer;

static J4AC_com_fang_myapplication_RaopServer class_J4AC_com_fang_myapplication_RaopServer;

void J4AC_com_fang_myapplication_RaopServer__videoSizeChange(JNIEnv *env, jint arg1, jint arg2)
{
    (*env)->CallStaticVoidMethod(env, class_J4AC_com_fang_myapplication_RaopServer.id, class_J4AC_com_fang_myapplication_RaopServer.method_videoSizeChange,  arg1, arg2);
}

void J4AC_com_fang_myapplication_RaopServer__videoSizeChange__catchAll(JNIEnv *env, jobject weakThiz, jint what, jint arg1, jint arg2, jobject obj)
{
    J4AC_com_fang_myapplication_RaopServer__videoSizeChange(env, arg1, arg2);
    J4A_ExceptionCheck__catchAll(env);
}

int J4A_loadClass__J4AC_com_fang_myapplication_RaopServer(JNIEnv *env){
    LOGD("J4A_loadClass__J4AC_com_fang_myapplication_RaopServer 0");
    int         ret                   = -1;
    const char *J4A_UNUSED(name)      = NULL;
    const char *J4A_UNUSED(sign)      = NULL;
    jclass      J4A_UNUSED(class_id)  = NULL;
    int         J4A_UNUSED(api_level) = 0;
    if (class_J4AC_com_fang_myapplication_RaopServer.id!=NULL){
        return 0;
    }
    sign = "com/fang/myapplication/RaopServer";
    class_J4AC_com_fang_myapplication_RaopServer.id = J4A_FindClass__asGlobalRef__catchAll(env,sign);
    if (class_J4AC_com_fang_myapplication_RaopServer.id == NULL){
        goto fail;
    }
    class_id = class_J4AC_com_fang_myapplication_RaopServer.id;
    name     = "videoSizeChange";
    sign     = "(II)V";
    class_J4AC_com_fang_myapplication_RaopServer.method_videoSizeChange = J4A_GetStaticMethodID__catchAll(env,class_id,name,sign);
    if (class_J4AC_com_fang_myapplication_RaopServer.method_videoSizeChange == NULL){
        goto fail;
    }

    LOGD("J4A_loadClass__J4AC_com_fang_myapplication_RaopServer 1");

    fail:
    return ret;
}