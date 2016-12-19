

struct file* file_open(const char* path, int flags, int rights);

int file_read(struct file* file, unsigned long long offset, unsigned char* data, unsigned int size);

int file_write(struct file* file, unsigned long long offset, unsigned char* data, unsigned int size);
