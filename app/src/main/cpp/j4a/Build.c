//
// Created by Lenovo on 2023/6/27.
//
#include "Build.h"

typedef struct J4AC_android_os_Build__VERSION {
    jclass id;

    jfieldID field_SDK_INT;
} J4AC_android_os_Build__VERSION;
static J4AC_android_os_Build__VERSION class_J4AC_android_os_Build__VERSION;

typedef struct J4AC_android_os_Build {
    jclass id;

} J4AC_android_os_Build;
static J4AC_android_os_Build class_J4AC_android_os_Build;

jint J4AC_android_os_Build__VERSION__SDK_INT__get(JNIEnv *env)
{
    return (*env)->GetStaticIntField(env, class_J4AC_android_os_Build__VERSION.id, class_J4AC_android_os_Build__VERSION.field_SDK_INT);
}

jint J4AC_android_os_Build__VERSION__SDK_INT__get__catchAll(JNIEnv *env)
{
    jint ret_value = J4AC_android_os_Build__VERSION__SDK_INT__get(env);
    if (J4A_ExceptionCheck__catchAll(env)) {
        return 0;
    }

    return ret_value;
}

void J4AC_android_os_Build__VERSION__SDK_INT__set(JNIEnv *env, jint value)
{
    (*env)->SetStaticIntField(env, class_J4AC_android_os_Build__VERSION.id, class_J4AC_android_os_Build__VERSION.field_SDK_INT, value);
}

void J4AC_android_os_Build__VERSION__SDK_INT__set__catchAll(JNIEnv *env, jint value)
{
    J4AC_android_os_Build__VERSION__SDK_INT__set(env, value);
    J4A_ExceptionCheck__catchAll(env);
}

int J4A_loadClass__J4AC_android_os_Build__VERSION(JNIEnv *env)
{
    int         ret                   = -1;
    const char *J4A_UNUSED(name)      = NULL;
    const char *J4A_UNUSED(sign)      = NULL;
    jclass      J4A_UNUSED(class_id)  = NULL;
    int         J4A_UNUSED(api_level) = 0;

    if (class_J4AC_android_os_Build__VERSION.id != NULL)
        return 0;

    sign = "android/os/Build$VERSION";
    class_J4AC_android_os_Build__VERSION.id = J4A_FindClass__asGlobalRef__catchAll(env, sign);
    if (class_J4AC_android_os_Build__VERSION.id == NULL)
        goto fail;

    class_id = class_J4AC_android_os_Build__VERSION.id;
    name     = "SDK_INT";
    sign     = "I";
    class_J4AC_android_os_Build__VERSION.field_SDK_INT = J4A_GetStaticFieldID__catchAll(env, class_id, name, sign);
    if (class_J4AC_android_os_Build__VERSION.field_SDK_INT == NULL)
        goto fail;

    J4A_ALOGD("J4ALoader: OK: '%s' loaded\n", "android.os.Build$VERSION");
    ret = 0;
    fail:
    return ret;
}

int J4A_loadClass__J4AC_android_os_Build(JNIEnv *env)
{
    int         ret                   = -1;
    const char *J4A_UNUSED(name)      = NULL;
    const char *J4A_UNUSED(sign)      = NULL;
    jclass      J4A_UNUSED(class_id)  = NULL;
    int         J4A_UNUSED(api_level) = 0;

    if (class_J4AC_android_os_Build.id != NULL)
        return 0;

    sign = "android/os/Build";
    class_J4AC_android_os_Build.id = J4A_FindClass__asGlobalRef__catchAll(env, sign);
    if (class_J4AC_android_os_Build.id == NULL)
        goto fail;

    ret = J4A_loadClass__J4AC_android_os_Build__VERSION(env);
    if (ret)
        goto fail;

    J4A_ALOGD("J4ALoader: OK: '%s' loaded\n", "android.os.Build");
    ret = 0;
    fail:
    return ret;
}
