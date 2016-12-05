#include <linux/ioctl.h>

typedef struct {
    int key;
    int value;
    int status;
} kvs_msg_t;

#define KVS_DEVICE_NAME "kvs"
#define KVS_MAJOR_NUM 300
#define IOCTL_KVS_PUT _IOWR(KVS_MAJOR_NUM, 0, kvs_msg_t *)
#define IOCTL_KVS_GET _IOWR(KVS_MAJOR_NUM, 1, kvs_msg_t *)
#define IOCTL_KVS_DEL _IOWR(KVS_MAJOR_NUM, 2, kvs_msg_t *)

#define KVS_BAD_ADDRESS 10
#define KVS_SUCCESS 0
#define KVS_FAIL 11
#define KVS_UNRECOGNIZED 12


static inline void serialize(int value, char dest[], int offset) {
    dest[offset] = (value >> 24) & 0xFF;
    dest[offset+1] = (value >> 16) & 0xFF;
    dest[offset+2] = (value >> 8) & 0xFF;
    dest[offset+3] = value & 0xFF;
}

static inline int deserialize(const char src[], int offset) {
    int ret = src[offset];
    ret = (ret << 8) + src[offset+1];
    ret = (ret << 8) + src[offset+2];
    ret = (ret << 8) + src[offset+3];

    return ret;
}

