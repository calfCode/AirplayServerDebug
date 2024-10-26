//
// Created by Lenovo on 2023/6/27.
//

#ifndef AIRPLAYSERVER_BUILD_H
#define AIRPLAYSERVER_BUILD_H
#include "j4a_base.h"
jint J4AC_android_os_Build__VERSION__SDK_INT__get(JNIEnv *env);
jint J4AC_android_os_Build__VERSION__SDK_INT__get__catchAll(JNIEnv *env);
void J4AC_android_os_Build__VERSION__SDK_INT__set(JNIEnv *env, jint value);
void J4AC_android_os_Build__VERSION__SDK_INT__set__catchAll(JNIEnv *env, jint value);
int J4A_loadClass__J4AC_android_os_Build(JNIEnv *env);
#endif //AIRPLAYSERVER_BUILD_H
