#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include "kvs_helper.h"
#include "kvs_ht.h"

#define DEVICE_NAME "kvs"

static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
long device_ioctl(struct file *file,
                  unsigned int ioctl_num,
                  unsigned long ioctl_param);
static long copy_msg_to_user(kvs_msg_t src, 
                             kvs_msg_t *dst); 

static struct file_operations fops = {
    .open = device_open,
    .release = device_release,
    .unlocked_ioctl = device_ioctl
};

static int device_open(struct inode *inode, struct file *file) {
    printk(KERN_INFO "device_open(%p)\n", file);
    return 0;
}

static int device_release(struct inode *inode, struct file *file) {
    printk(KERN_INFO "device_release(%p,%p)\n", inode, file);
    return 0;
}

long device_ioctl(struct file *file,
                 unsigned int ioctl_num,
                 unsigned long ioctl_param)
{
    kvs_msg_t *dst = (kvs_msg_t *) ioctl_param;
    kvs_msg_t src;

    if (!get_user(src.key, &(dst->key)))
        return KVS_BAD_ADDRESS;
    if (!get_user(src.value, &(dst->value)))
        return KVS_BAD_ADDRESS;
    src.status = KVS_SUCCESS;

    switch (ioctl_num) {
    case IOCTL_KVS_PUT:
        if (!kvs_ht_put(src.key, src.value))
            src.status = KVS_FAIL;
        return copy_msg_to_user(src, dst);
    case IOCTL_KVS_GET:
        if (!kvs_ht_get(src.key, &(src.value)))
            src.status = KVS_FAIL;
        return copy_msg_to_user(src, dst);
    case IOCTL_KVS_DEL:
        if (!kvs_ht_remove(src.key))
            src.status = KVS_FAIL;
        return copy_msg_to_user(src, dst);
    default:
        return KVS_UNRECOGNIZED;
    }
}

static long copy_msg_to_user(kvs_msg_t src, 
                             kvs_msg_t *dst) 
{
    if (!put_user(src.key, &(dst->key)))
        return KVS_BAD_ADDRESS;
    if (!put_user(src.value, &(dst->value)))
        return KVS_BAD_ADDRESS;
    if (!put_user(src.value, &(dst->status)))
        return KVS_BAD_ADDRESS;
    return KVS_SUCCESS;
}

static int __init onload(void) {
    int ret_val;
    ret_val = register_chrdev(KVS_MAJOR_NUM, DEVICE_NAME, &fops);
    if (ret_val < 0)
        printk(KERN_ALERT "Registering the character device failed with %d.", ret_val);
    printk(KERN_EMERG "Loadable module initialized\n");
    return 0;
}

static void __exit onunload(void) {
    printk(KERN_EMERG "Loadable module removed\n");
}

module_init(onload);
module_exit(onunload);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mikael Karlsson, Jesper Bilander, Mattias Scherer");
MODULE_DESCRIPTION("A simple key value store");
