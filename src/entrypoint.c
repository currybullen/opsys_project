#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include "kvs_helper.h"
#include "kvs_ht.h"

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

static struct class* kvs_char_class   = NULL; // The device-driver cass struct pointer
static struct device* kvs_char_device = NULL; // The device-driver device struct pointer
static DEFINE_MUTEX(kvs_mutex);

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

    printk(KERN_INFO "Copying parameters info to kernel space.\n");
    if (get_user(src.key, &(dst->key)))
        return KVS_BAD_ADDRESS;
    if (get_user(src.value, &(dst->value)))
        return KVS_BAD_ADDRESS;
    src.status = KVS_SUCCESS;

    printk(KERN_INFO "Copying done!\n");

    switch (ioctl_num) {
    case IOCTL_KVS_PUT:
        printk(KERN_INFO "Performing KVS_PUT action\n");
        if (!kvs_ht_put(src.key, src.value, NULL))
            src.status = KVS_FAIL;
        return copy_msg_to_user(src, dst);
    case IOCTL_KVS_GET:
        printk(KERN_INFO "Performing KVS_GET action\n");
        if (!kvs_ht_get(src.key, &(src.value)))
            src.status = KVS_FAIL;
        return copy_msg_to_user(src, dst);
    case IOCTL_KVS_DEL:
        printk(KERN_INFO "Performing KVS_DEL action\n");
        if (!kvs_ht_remove(src.key, NULL))
            src.status = KVS_FAIL;
        return copy_msg_to_user(src, dst);
    default:
        return KVS_UNRECOGNIZED;
    }
}

static long copy_msg_to_user(kvs_msg_t src,
                             kvs_msg_t *dst)
{
    printk(KERN_INFO "Copying return values to user space.\n");
    if (put_user(src.key, &(dst->key)))
        return KVS_BAD_ADDRESS;
    if (put_user(src.value, &(dst->value)))
        return KVS_BAD_ADDRESS;
    if (put_user(src.status, &(dst->status)))
        return KVS_BAD_ADDRESS;
    return KVS_SUCCESS;
}

static int __init onload(void) {
    int ret_val;

    printk(KERN_INFO "Initializing the KVSchar LKM\n");

    ret_val = register_chrdev(KVS_MAJOR_NUM, KVS_DEVICE_NAME, &fops);
    if (ret_val < 0) {
        printk(KERN_ALERT "Registering character device %s with major %d failed with %d.\n", KVS_DEVICE_NAME, KVS_MAJOR_NUM, ret_val);
        return ret_val;
    }
    else {
        printk(KERN_INFO "Successfully registered character device %s with major %d\n", KVS_DEVICE_NAME, KVS_MAJOR_NUM);
    }

    /* Register the device class */
    kvs_char_class = class_create(THIS_MODULE, KVS_CLASS_NAME);
    if (IS_ERR(kvs_char_class)) {
        unregister_chrdev(KVS_MAJOR_NUM, KVS_DEVICE_NAME);
        printk(KERN_ALERT "Failed to register device class\n");
        return PTR_ERR(kvs_char_class);
    }
    /* Register the device driver */
    kvs_char_device = device_create(kvs_char_class, NULL, MKDEV(KVS_MAJOR_NUM, 0), NULL, KVS_DEVICE_NAME);
    if (IS_ERR(kvs_char_device)) {
        class_destroy(kvs_char_class);
        unregister_chrdev(KVS_MAJOR_NUM, KVS_DEVICE_NAME);
        printk(KERN_ALERT "Failed to create the device\n");
        return PTR_ERR(kvs_char_device);
    }

    kvs_ht_init();
    printk(KERN_EMERG "Loadable module initialized\n");
    return 0;
}

static void __exit onunload(void) {
    device_destroy(kvs_char_class, MKDEV(KVS_MAJOR_NUM, 0));
    class_unregister(kvs_char_class);
    class_destroy(kvs_char_class);
    unregister_chrdev(KVS_MAJOR_NUM, KVS_DEVICE_NAME);
    kvs_ht_cleanup();
    printk(KERN_INFO "Successfully unregistered character device %s with major %d.\n", KVS_DEVICE_NAME, KVS_MAJOR_NUM);
    printk(KERN_EMERG "Loadable module removed\n");
}

module_init(onload);
module_exit(onunload);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mikael Karlsson, Jesper Bilander, Mattias Scherer");
MODULE_DESCRIPTION("A simple key value store");
