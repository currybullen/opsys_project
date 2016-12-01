#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include "kvs_ht.h"

static int __init onload(void) {
    printk(KERN_EMERG "Loadable module initialized\n");
    return 0;
}

static void __exit onunload(void) {
    printk(KERN_EMERG "Loadable module removed\n");
}

static void run_tests(void) {
    int value = 5;
    int key = 7;
    int fetched_value = 0;
    printk(KERN_INFO "Storing value %d with key %d.\n", value, key);
    kvs_ht_put(value, key);

    fetched_value = *kvs_ht_get(key);
    printk(KERN_INFO "Found value  %d for key %d.\n.", fetched_value, key);
}

module_init(onload);
module_exit(onunload);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mikael Karlsson, Jesper Bilander, Mattias Scherer");
MODULE_DESCRIPTION("A simple key value store");
