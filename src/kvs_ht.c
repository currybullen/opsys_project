#include <linux/module.h>  /* Needed by all kernel modules */
#include <linux/kernel.h>  /* Needed for loglevels (KERN_WARNING, KERN_EMERG, KERN_INFO, etc.) */
#include <linux/init.h>    /* Needed for __init and __exit macros. */
#include <linux/slab.h>    /* kmalloc */
#include <linux/hashtable.h>
#include "kvs_ht.h"

static DEFINE_HASHTABLE(ht, 5);

void put(int value, int key) {
    kvs_entry_t kvs_entry = {
        .value = value,
        .key = key
    };

    hash_add(ht, &kvs_entry.next, key);
}

int get(int key) {
    kvs_entry_t *kvs_entry;
    hash_for_each_possible(ht, kvs_entry, next, key) {
        if (kvs_entry->key == key)
            return kvs_entry->value;
    }

    return -1;
}
