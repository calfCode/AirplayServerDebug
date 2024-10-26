//
// Created by Lenovo on 2023/6/27.
//

#ifndef AIRPLAYSERVER_ANDROID_JNI_H
#define AIRPLAYSERVER_ANDROID_JNI_H

#include "j4a_base.h"
#include "j4a/j4a_allclasses.h"
JavaVM *SDL_JNI_GetJvm();

jint    SDL_JNI_SetupThreadEnv(JNIEnv **p_env);
void    SDL_JNI_DetachThreadEnv();

int      SDL_JNI_ThrowException(JNIEnv *env, const char *exception, const char* msg);
int      SDL_JNI_ThrowIllegalStateException(JNIEnv *env, const char* msg);

jobject SDL_JNI_NewObjectAsGlobalRef(JNIEnv *env, jclass clazz, jmethodID methodID, ...);

void    SDL_JNI_DeleteGlobalRefP(JNIEnv *env, jobject *obj_ptr);
void    SDL_JNI_DeleteLocalRefP(JNIEnv *env, jobject *obj_ptr);

int     SDL_Android_GetApiLevel();

#define JNI_FIND_JAVA_CLASS(env__, var__, classsign__) \
    do { \
        jclass clazz = (*env__)->FindClass(env__, classsign__); \
        if (J4A_ExceptionCheck__catchAll(env) || !(clazz)) { \
            ALOGE("FindClass failed: %s", classsign__); \
            return -1; \
        } \
        var__ = (*env__)->NewGlobalRef(env__, clazz); \
        if (J4A_ExceptionCheck__catchAll(env) || !(var__)) { \
            ALOGE("FindClass::NewGlobalRef failed: %s", classsign__); \
            (*env__)->DeleteLocalRef(env__, clazz); \
            return -1; \
        } \
        (*env__)->DeleteLocalRef(env__, clazz); \
    } while(0);

#define JNI_CHECK_GOTO(condition__, env__, exception__, msg__, label__) \
    do { \
        if (!(condition__)) { \
            if (exception__) { \
                SDL_JNI_ThrowException(env__, exception__, msg__); \
            } \
            goto label__; \
        } \
    }while(0)

#define JNI_CHECK_RET_VOID(condition__, env__, exception__, msg__) \
    do { \
        if (!(condition__)) { \
            if (exception__) { \
                SDL_JNI_ThrowException(env__, exception__, msg__); \
            } \
            return; \
        } \
    }while(0)

#define JNI_CHECK_RET(condition__, env__, exception__, msg__, ret__) \
    do { \
        if (!(condition__)) { \
            if (exception__) { \
                SDL_JNI_ThrowException(env__, exception__, msg__); \
            } \
            return ret__; \
        } \
    }while(0)
#endif //AIRPLAYSERVER_ANDROID_JNI_H
