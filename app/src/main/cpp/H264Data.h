//
// Created by Lenovo on 2023/3/13.
//

#ifndef AIRPLAYSERVER_H264DATA_H
#define AIRPLAYSERVER_H264DATA_H
typedef struct {
    unsigned char *data;
    int data_len;
    uint64_t pts;
} h264_data_struct;
#endif //AIRPLAYSERVER_H264DATA_H
