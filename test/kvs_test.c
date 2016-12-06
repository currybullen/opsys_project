#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include "kvs_helper.h"


static char* KVS_PATH = "/dev/kvs";
static int fd;

void test_put(int key, int value);
void test_get(int key);
void test_del(int key);

int main(int argc, char *argv[]) {
    if (argc > 1)
        KVS_PATH = argv[1];
    fd = open(KVS_PATH, 0);
    if (fd < 0) {
        printf("Can't open device file %s\n", KVS_PATH);
        exit(-1);
    }
    test_del(0);
    int val = 0;
    /*
    while(1) {
        test_put(0, val++);
        test_del(0);
    }*/
    while (1) {
        test_get(0);
    }
    //test_get(0);
}

void test_put(int key, int value) {
    kvs_msg_t msg;
    msg.key = key;
    msg.value = value;

    printf("Attempting to store value %d behind key %d.\n", msg.key, msg.value);
    int ret_val = ioctl(fd, IOCTL_KVS_PUT, &msg);
    if (ret_val == KVS_SUCCESS) {
        printf("Put call returned success!\n");
        if (msg.status == KVS_SUCCESS)
            printf("Returned message contained success code! Stored value %d behind key %d.\n", value, key);
        else
            printf("Returned message contained fault code, could not store value %d behind key %d.\n", value, key);
    } else {
        printf("Put call returned %d, something is wrong.\n", ret_val);
    }
}

void test_get(int key) {
    kvs_msg_t msg;
    msg.key = key;

    printf("Attempting to retrieve value behind key %d.\n", key);
    int ret_val = ioctl(fd, IOCTL_KVS_GET, &msg);
    if (ret_val == KVS_SUCCESS) {
        printf("Get call returned success!\n");
        if (msg.status == KVS_SUCCESS)
            printf("Returned message contained success code! Retrieved value %d for key %d.\n", msg.value, key);
        else
            printf("Returned message contained fault code, could not retrieve value behind key %d.\n", key);
    } else {
        printf("Get call returned %d, something is wrong.\n", ret_val);
    }
}

void test_del(int key) {
    kvs_msg_t msg;
    msg.key = key;

    printf("Attempting to delete value behind key %d.\n", key);
    int ret_val = ioctl(fd, IOCTL_KVS_DEL, &msg);
    if (ret_val == KVS_SUCCESS) {
        printf("Del call returned success!\n");
        if (msg.status == KVS_SUCCESS)
            printf("Returned message contained success code! Deleted value behind key %d.\n", key);
        else
            printf("Returned message contained fault code, could not delete value behind key %d\n", key);
    } else {
        printf("Del call returned %d, something is wrong.\n", ret_val);
    }
}
