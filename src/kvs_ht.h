bool kvs_ht_put(int key, int value, int *old_value);

bool kvs_ht_get(int key, int* value);

bool kvs_ht_remove(int key, int *old_value);

bool kvs_ht_entryset(int *keys, int *values, size_t size);

size_t kvs_ht_size(void);

void kvs_ht_init(void);

void kvs_ht_cleanup(void);
