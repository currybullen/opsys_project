#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include "kvs_helper.h"
#include "kvs_ht.h"

static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

#define DEVICE_NAME "kvs"
#define BUF_LEN 10

static int kvs_major;
static bool kvs_open = false;
static char msg[BUF_LEN];
static int msg_write_size;

static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release
};

static int device_open(struct inode *inode, struct file *file) {
    return 0;
}

static int device_release(struct inode *inode, struct file *file) {
    return 0;
}


static ssize_t device_read(struct file *filp, 
                          char *buffer, 
                          size_t length, 
                          loff_t *offset)
{
    //copy from global buffer to parameter buffer
    return 0;
}

static ssize_t device_write(struct file *filp, 
                            const char *buffer, 
                            size_t len, 
                            loff_t *offset) 
{
    if (len == 0)
        //return KVS_BAD_FORMAT

    switch(buffer[0])
    {
        case KVS_PUT :
            //handle_put(buff, len, msg);
            break;

        case KVS_GET :
            //handle_get(buff, len, msg);
            break;

        case KVS_DEL :
            //handle_del(buff, len, msg);
            break;

        default :
            ;

    }

    return 0;
}


static int __init onload(void) {
    register_chrdev(0, DEVICE_NAME, &fops);
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
