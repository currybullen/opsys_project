#include <linux/module.h>  /* Needed by all kernel modules */
#include <linux/kernel.h>  /* Needed for loglevels (KERN_WARNING, KERN_EMERG, KERN_INFO, etc.) */
#include <linux/init.h>    /* Needed for __init and __exit macros. */
#include <linux/slab.h>    /* kmalloc */
#include <linux/hashtable.h>
#include "kvs_ht.h"

typedef struct {
    int key;
    int value;
    struct hlist_node next;
} ht_entry_t;

static DEFINE_HASHTABLE(ht, 5);

void kvs_ht_put(int value, int key) {
    ht_entry_t *ht_entry = kmalloc(sizeof(ht_entry_t), GFP_KERNEL);
    ht_entry->key = key;
    ht_entry->value = value;

    if (kvs_ht_get(key) != NULL)
        kvs_ht_remove(key);

    hash_add(ht, &(ht_entry->next), key);
}

int* kvs_ht_get(int key) {
    ht_entry_t *ht_entry;
    hash_for_each_possible(ht, ht_entry, next, key) {
        if (ht_entry->key == key)
            return &(ht_entry->value);
    }

    return NULL;
}

void kvs_ht_remove(int key) {
    
}
