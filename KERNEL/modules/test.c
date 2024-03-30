#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
//#include <linux/proc_fs.h>
#include <linux/rwlock.h>
#include <linux/sysfs.h>
#include <linux/string.h>
//#include <linux/kobject.h>


static int major = 0;
//static struct proc_dir_entry *test = NULL;
//static struct kobject *test_kobj = NULL;
static rwlock_t lock;
static char test_string[15] = "Hello\n\0";


ssize_t test_read(struct file *fd, char __user *buff, size_t size,
        loff_t *off) {

        size_t rc;

        read_lock(&lock);
        rc = simple_read_from_buffer(buff, size, off, test_string, 15);
        read_unlock(&lock);

        return rc;
}


ssize_t test_write(struct file *fd, const char __user *buff, size_t size,
        loff_t *off) {
        size_t rc = 0;

        if (size > 15)
          return -EINVAL;

        write_lock(&lock);
        rc = simple_write_to_buffer(test_string, 15, off, buff, size);
        write_unlock(&lock);

        return rc;
}



static struct file_operations fops = {
        .owner = THIS_MODULE,
        .read = test_read,
        .write = test_write
};



int init_module(void) {
        int retval = 0;
        pr_info("My test module loaded!!!\n");
        rwlock_init(&lock);
        major = register_chrdev(major, "test", &fops);

        if (major < 0) return major;
        pr_info("Major = %d\n", major);

        return retval;
}


void cleanup_module(void) {
        unregister_chrdev(major, "test");

        pr_info("My test module unloaded!!!\n");
}



MODULE_LICENSE("GPL");

