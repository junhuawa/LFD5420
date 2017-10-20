/* **************** LFD5420:4.13 s_05/trivial.c **************** */
/*
 * The code herein is: Copyright the Linux Foundation, 2017
 *
 * This Copyright is retained for the purpose of protecting free
 * redistribution of source.
 *
 *     URL:    http://training.linuxfoundation.org
 *     email:  trainingquestions@linuxfoundation.org
 *
 * This code is distributed under Version 2 of the GNU General Public
 * License, which you should have received with the source.
 *
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

static char *ramdisk;
#define ramdisk_size (size_t) (4 * PAGE_SIZE)

static dev_t first;
static int my_major=700, my_minor=0;
static struct cdev *my_timer_cdev;
static unsigned int count = 1;

struct timer_list my_timer;

#define MYDEV_NAME "mytimercdev"

static void my_timer_handler(unsigned long val)
{
	
    mod_timer(&my_timer, jiffies + 10* HZ);

    printk("timer expire! %s, current pid: %d\n", ramdisk, current->pid);
    return;
}

static int timer_open(struct inode *inode, struct file *file)
{
	pr_info("timer open called\n");

    return 0;
}

static int timer_release(struct inode *inode, struct file *file)
{
	pr_info("dev timer release called\n");

    return 0;
}

static ssize_t timer_read(struct file *file, char __user *buf, size_t sz, loff_t *ppos)
{
	pr_info("timer read called, len: %d\n", (int)sz);

	if(sz > ramdisk_size) sz = ramdisk_size;

	if(copy_to_user(buf, ramdisk, sz)){
		return -EFAULT;
	}
	pr_info("Reading nbytes: %d, pos: %d, current pid: %d\n", (int)sz, (int)*ppos, current->pid);
	return sz;
}

static ssize_t timer_write(struct file *file, const char __user *buf, size_t sz, loff_t *ppos)
{

	if(sz > ramdisk_size) sz = ramdisk_size;
	if(copy_from_user(ramdisk, buf, sz)){
		return -EFAULT;
	}

	pr_info("timer write %s length %d, current pid: %d\n", ramdisk, (int)sz, current->pid);

	init_timer(&my_timer);
	my_timer.expires = jiffies + 3*HZ;
	my_timer.function = my_timer_handler;

	add_timer(&my_timer);
	return sz;
}

static const struct file_operations timer_fops = {
	.owner = THIS_MODULE,
	.read = timer_read,
	.write = timer_write,
	.open = timer_open,
	.release = timer_release,
};

static int __init my_init(void)
{
	ramdisk = kzalloc(ramdisk_size, GFP_KERNEL);

	first = MKDEV(my_major, my_minor);
	register_chrdev_region(first, count, MYDEV_NAME);

	my_timer_cdev = cdev_alloc();
	cdev_init(my_timer_cdev, &timer_fops);
	cdev_add(my_timer_cdev, first, count);

	pr_info("Succeed in registering character device %s\n", MYDEV_NAME);
	return 0;
}

static void __exit my_exit(void)
{
	del_timer_sync(&my_timer);
	kfree(ramdisk);
	cdev_del(my_timer_cdev);
	unregister_chrdev_region(first, count);

	pr_info("Bye: module unloaded from 0x%p\n", my_exit);
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("A GENIUS");
MODULE_LICENSE("GPL v2");
