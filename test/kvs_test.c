#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>

#define KVS_PUT 1
#define KVS_PUT_OK 2
#define KV_PUT_FAIL 3

#define KVS_GET 4
#define KVS_GET_OK 5
#define KVS_GET_FAIL 6

#define KVS_DEL 7
#define KVS_DEL_OK 8
#define KVS_DEL_FAIL = 9

static char* KVS_PATH = "/dev/kvs";

bool put(int key, int value);
bool get(int key, int *value);
bool delete(int key);
void run_tests();

static void serialize(int value, char dest[], int offset);
static int deserialize(char src[], int offset);
static void write_read(char write_buf[], int write_size, 
        char read_buf[], int read_size);

int main(int argc, char *argv[]) {
    if (argc > 1)
        KVS_PATH = argv[1];
    run_tests();
}

void run_tests() {
    int key = 7;
    int value = 5;
    printf("Storing value %d with key %d\n", value, key);
    put(key, value);

    get(key, &value);
    printf("Found value %d behind key %d\n", value, key);
}

bool put(int key, int value) {
    char buffer[9];
    buffer[0] = KVS_PUT;
    serialize(key, buffer, 1);
    serialize(value, buffer, 5);
    write_read(buffer, 9, buffer, 9);

    return (buffer[0] == KVS_PUT_OK);
}

bool get(int key, int* value) {
    char buffer[5];
    buffer[0] = KVS_GET;
    serialize(key, buffer, 1);
    write_read(buffer, 5, buffer, 5);
    if (buffer[0] == KVS_GET_OK) {
        *value = deserialize(buffer, 1);
        return true;
    }

    return false;
}

bool delete(int key) {
    char buffer[5];
    buffer[0] = KVS_DEL;
    serialize(key, buffer, 1);
    write_read(buffer, 5, buffer, 5);
    
    return (buffer[0] == KVS_DEL_OK);
}

void serialize(int value, char dest[], int offset) {
    dest[offset] = (value >> 24) & 0xFF;
    dest[offset+1] = (value >> 16) & 0xFF;
    dest[offset+2] = (value >> 8) & 0xFF;
    dest[offset+3] = value & 0xFF;
}

static int deserialize(char src[], int offset) {
    int ret = src[offset];
    ret = (ret << 8) + src[offset+1];
    ret = (ret << 8) + src[offset+2];
    ret = (ret << 8) + src[offset+3];

    return ret;
}

static void write_read(char write_buf[], int write_size, 
        char read_buf[], int read_size) {
    int fd = open(KVS_PATH, O_RDWR);
    write(fd, write_buf, write_size);
    read(fd, read_buf, read_size);
    close(fd);
}
