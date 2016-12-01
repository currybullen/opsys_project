#include <linux/module.h>  /* Needed by all kernel modules */
#include <linux/kernel.h>  /* Needed for loglevels (KERN_WARNING, KERN_EMERG, KERN_INFO, etc.) */
#include <linux/init.h>    /* Needed for __init and __exit macros. */
#include <linux/slab.h>    /* kmalloc */
#include <linux/hashtable.h>
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
    put(value, key);

    fetched_value = get(key);
    printk(KERN_INFO "Found value  %d for key %d.\n.", fetched_value, key);
}

module_init(onload);
module_exit(onunload);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mikael Karlsson, Jesper Bilander, Mattias Scherer");
MODULE_DESCRIPTION("A simple key value store");
