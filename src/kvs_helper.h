#define KVS_PUT 1
#define KVS_PUT_OK 2
#define KV_PUT_FAIL 3

#define KVS_GET 4
#define KVS_GET_OK 5
#define KVS_GET_FAIL 6

#define KVS_DEL 7
#define KVS_DEL_OK 8
#define KVS_DEL_FAIL = 9

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

typedef struct {
    int key;
    int value;
    int status
} kvs_msg;
