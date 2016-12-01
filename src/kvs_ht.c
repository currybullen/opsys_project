#include <linux/slab.h>
#include <linux/hashtable.h>
#include "kvs_ht.h"

typedef struct {
    int key;
    int value;
    struct hlist_node next;
} ht_entry_t;

static DEFINE_HASHTABLE(ht, 5);

static bool replace_if_present(int key, int value) {
    ht_entry_t* ht_entry;
    hash_for_each_possible(ht, ht_entry, next, key) {
        if (ht_entry->key == key) {
            ht_entry->value = value;
            return true;
        }
    }

    return false;
}

void kvs_ht_put(int key, int value) {
    ht_entry_t* ht_entry;

    if (replace_if_present(key,value))
        return;
    
    ht_entry = kmalloc(sizeof(ht_entry_t), GFP_KERNEL);
    ht_entry->key = key;
    ht_entry->value = value;

    hash_add(ht, &(ht_entry->next), key);
}

int* kvs_ht_get(int key) {
    ht_entry_t* ht_entry;
    hash_for_each_possible(ht, ht_entry, next, key) {
        if (ht_entry->key == key)
            return &(ht_entry->value);
    }

    return NULL;
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
