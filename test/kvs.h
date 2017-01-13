#define SUCCESS 0
#define FAIL 1
#define BAD_PATH 2
#define INTERNAL_ERROR 3

int put(int fd, int key, int value);

int get(int fd, int key, int *value);

int delete(int fd, int key);
