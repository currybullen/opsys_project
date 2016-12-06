#include <linux/slab.h>
#include <linux/hashtable.h>
#include <linux/mutex.h>
#include "kvs_ht.h"

#define KVS_HT_SIZE 5
static DEFINE_HASHTABLE(ht, KVS_HT_SIZE);
static struct mutex kvs_locks[32];

typedef struct {
    int key;
    int value;
    struct hlist_node next;
} ht_entry_t;

bool kvs_ht_put(int key, int value, int *old_value) {
    ht_entry_t* ht_entry;
    int tmp;
    int bucket = hash_min(key, KVS_HT_SIZE);

    if (kvs_ht_get(key, &tmp)) {
        if (old_value != NULL)
            *old_value = tmp;
        return false;
    }

    ht_entry = kmalloc(sizeof(ht_entry_t), GFP_KERNEL);
    ht_entry->key = key;
    ht_entry->value = value;
    mutex_lock(&kvs_locks[bucket]);
    hash_add(ht, &(ht_entry->next), key);
    mutex_unlock(&kvs_locks[bucket]);
    return true;
}

bool kvs_ht_get(int key, int* value) {
    ht_entry_t* ht_entry;
    int bucket = hash_min(key, KVS_HT_SIZE);

    mutex_lock(&kvs_locks[bucket]);
    hash_for_each_possible(ht, ht_entry, next, key) {
        if (ht_entry->key == key) {
            *value = ht_entry->value;
            mutex_unlock(&kvs_locks[bucket]);
            return true;
        }
    }
    mutex_unlock(&kvs_locks[bucket]);
    return false;
}

bool kvs_ht_remove(int key, int *old_value) {
    ht_entry_t* ht_entry;
    int bucket = hash_min(key, KVS_HT_SIZE);

    mutex_lock(&kvs_locks[bucket]);
    hash_for_each_possible(ht, ht_entry, next, key) {
        if (ht_entry->key == key) {
            if (old_value != NULL)
                *old_value = ht_entry->value;
            hash_del(&(ht_entry->next));
            kfree(ht_entry);
            mutex_unlock(&kvs_locks[bucket]);
            return true;
        }
    }
    mutex_unlock(&kvs_locks[bucket]);
    return false;
}

void kvs_ht_init(void) {
    int i;
    for (i = 0; i < 32; i++) {
        mutex_init(&kvs_locks[i]);
    }
}

void kvs_ht_cleanup(void) {
    ht_entry_t* ht_entry;
    int bkt;
    hash_for_each(ht, bkt, ht_entry, next) {
        hash_del(&(ht_entry->next));
        kfree(ht_entry);
    }
}
