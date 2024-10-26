//
// Created by Lenovo on 2023/6/27.
//

#ifndef AIRPLAYSERVER_RAOPSERVER_JNI_H
#define AIRPLAYSERVER_RAOPSERVER_JNI_H
#include "j4a/j4a_base.h"

void J4AC_com_fang_myapplication_RaopServer__videoSizeChange(JNIEnv *env, jint arg1, jint arg2);
void J4AC_com_fang_myapplication_RaopServer__videoSizeChange__catchAll(JNIEnv *env, jobject weakThiz, jint what, jint arg1, jint arg2, jobject obj);
#define J4AC_RaopServer__videoSizeChange__catchAll J4AC_com_fang_myapplication_RaopServer__videoSizeChange__catchAll
#define J4AC_RaopServer__videoSizeChange J4AC_com_fang_myapplication_RaopServer__videoSizeChange
int J4A_loadClass__J4AC_com_fang_myapplication_RaopServer(JNIEnv *env);
#define J4A_loadClass__J4AC_RaopServer J4A_loadClass__J4AC_com_fang_myapplication_RaopServer
#endif //AIRPLAYSERVER_RAOPSERVER_JNI_H
