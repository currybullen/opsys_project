#include <linux/slab.h>
#include <linux/hashtable.h>
#include "kvs_ht.h"

#define KVS_HT_SIZE 5
static DEFINE_HASHTABLE(ht, KVS_HT_SIZE);

typedef struct {
    int key;
    int value;
    struct hlist_node next;
} ht_entry_t;

bool kvs_ht_put(int key, int value) {
    ht_entry_t* ht_entry;
    int tmp;

    if (kvs_ht_get(key, &tmp) && tmp == value) 
        return false;

    ht_entry = kmalloc(sizeof(ht_entry_t), GFP_KERNEL);
    ht_entry->key = key;
    ht_entry->value = value;
    hash_add(ht, &(ht_entry->next), key);

    return true;
}

bool kvs_ht_get(int key, int* value) {
    ht_entry_t* ht_entry;
    hash_for_each_possible(ht, ht_entry, next, key) {
        if (ht_entry->key == key) {
            *value = ht_entry->value;
            return true;
        }
    }

    return false;
}

bool kvs_ht_remove(int key) {
    ht_entry_t* ht_entry;
    hash_for_each_possible(ht, ht_entry, next, key) {
        if (ht_entry->key == key) {
            hash_del(&(ht_entry->next));
            kfree(ht_entry);
            return true;
        }
    }

    return false;
}

void kvs_ht_cleanup(void) {
    ht_entry_t* ht_entry;
    int bkt;
    hash_for_each(ht, bkt, ht_entry, next) {
        hash_del(&(ht_entry->next));
        kfree(ht_entry);
    }
}
