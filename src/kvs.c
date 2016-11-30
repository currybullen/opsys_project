#include <linux/module.h>  /* Needed by all kernel modules */
#include <linux/kernel.h>  /* Needed for loglevels (KERN_WARNING, KERN_EMERG, KERN_INFO, etc.) */
#include <linux/init.h>    /* Needed for __init and __exit macros. */
#include <linux/slab.h>    /* kmalloc */
#include <linux/hashtable.h>
#include "kvs.h"

//static int HT_SIZE = 8;
static DEFINE_HASHTABLE(ht, 5);

static int __init onload(void) {
    printk(KERN_EMERG "Loadable module initialized\n");
    return 0;
}

static void __exit onunload(void) {
    printk(KERN_EMERG "Loadable module removed\n");
}

void run_tests(void) {
    int value = 5;
    int key = 7;
    int fetched_value = 0;
    printk(KERN_INFO "Storing value %d with key %d.\n", value, key);
    put(value, key);

    fetched_value = get(key);
    printk(KERN_INFO "Found value  %d for key %d.\n.", fetched_value, key);
}

//make static? also implement mem allocation here
void put(int value, int key) {
    kvs_entry_t kvs_entry = {
        .value = value,
        .key = key
    };

    hash_add(ht, &kvs_entry.next, key);
}

//make static, maybe shouldn't return -1?
int get(int key) {
    kvs_entry_t *kvs_entry;
    hash_for_each_possible(ht, kvs_entry, next, key) {
        if (kvs_entry->key == key)
            return kvs_entry->value;
    }

    return -1;
}

module_init(onload);
module_exit(onunload);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mikael Karlsson, Jesper Bilander, Mattias Scherer");
MODULE_DESCRIPTION("A simple key value store");
