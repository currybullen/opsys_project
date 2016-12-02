bool kvs_ht_put(int key, int value);

bool kvs_ht_get(int key, int* value);

bool kvs_ht_remove(int key);

void kvs_ht_cleanup(void);
