#include <linux/module.h>  /* Needed by all kernel modules */
#include <linux/kernel.h>  /* Needed for loglevels (KERN_WARNING, KERN_EMERG, KERN_INFO, etc.) */
#include <linux/init.h>    /* Needed for __init and __exit macros. */
#include <linux/slab.h>    /* kmalloc */
#include <linux/hashtable.h>

void kvs_ht_put(int value, int key);

int* kvs_ht_get(int key);

void kvs_ht_remove(int key);
