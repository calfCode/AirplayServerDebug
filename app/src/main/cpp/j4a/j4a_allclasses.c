//
// Created by Lenovo on 2023/6/27.
//
#include "j4a_allclasses.h"

int J4A_LoadAll__catchAll(JNIEnv *env)
{
    int ret = 0;

    J4A_LOAD_CLASS(com_fang_myapplication_RaopServer);

    fail:
    return ret;
}
