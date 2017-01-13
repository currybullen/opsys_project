#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "kvs.h"
#include "kvs_helper.h"

int put(int fd, int key, int value) {
    kvs_msg_t msg;
    msg.key = key;
    msg.value = value;
    
    int ret_val = ioctl(fd, IOCTL_KVS_PUT, &msg);

    if (ret_val != KVS_SUCCESS)
        return INTERNAL_ERROR;
    return (msg.status == KVS_SUCCESS) ? SUCCESS : FAIL;
}

int get(int fd, int key, int* value) {
    kvs_msg_t msg;
    msg.key = key;

    int ret_val = ioctl(fd, IOCTL_KVS_GET, &msg);
    *value = msg.value;

    if (ret_val != KVS_SUCCESS)
        return INTERNAL_ERROR;
    return (msg.status == KVS_SUCCESS) ? SUCCESS : FAIL;
}

int delete(int fd, int key) {
    kvs_msg_t msg;
    msg.key = key;

    int ret_val = ioctl(fd, IOCTL_KVS_DEL, &msg);

    if (ret_val != KVS_SUCCESS)
        return INTERNAL_ERROR;
    return (msg.status == KVS_SUCCESS) ? SUCCESS : FAIL;
}
