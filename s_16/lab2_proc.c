/* **************** LFD5420:4.13 s_16/lab2_proc.c **************** */
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
/*

* Using the /proc filesystem.
*
* Write a module that creates a /proc filesystem entry and can read
* and write to it.
*
* When you read from the entry, you should obtain the value of some
* parameter set in your module.
*
* When you write to the entry, you should modify that value, which
* should then be reflected in a subsequent read.
*
* Make sure you remove the entry when you unload your module.  What
* happens if you don't and you try to access the entry after the
* module has been removed?
*
* The solution shows how to create the entry in the /proc directory
* and also in the /proc/driver directory.
*
@*/

#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/init.h>

#if 0
#define NODE "my_proc"
#else
#define NODE "driver/my_proc"
#endif

static int param = 100;
static struct proc_dir_entry *my_proc;

#define KS 32
static char kstring[KS];	/* should be less sloppy about overflows :) */
static ssize_t
my_read(struct file *file, char __user *buf, size_t lbuf, loff_t *ppos)
{
	int nbytes = sprintf(kstring, "%d\n", param);
	return simple_read_from_buffer(buf, lbuf, ppos, kstring, nbytes);
}

static ssize_t my_write(struct file *file, const char __user *buf, size_t lbuf,
			loff_t *ppos)
{
	ssize_t rc;
	rc = simple_write_to_buffer(kstring, lbuf, ppos, buf, lbuf);
	sscanf(kstring, "%d", &param);
	pr_info("param has been set to %d\n", param);
	return rc;
}

static const struct file_operations my_proc_fops = {
	.owner = THIS_MODULE,
	.read = my_read,
	.write = my_write,
};

static int __init my_init(void)
{
	my_proc = proc_create(NODE, S_IRUGO | S_IWUSR, NULL, &my_proc_fops);
	if (!my_proc) {
		pr_err("I failed to make %s\n", NODE);
		return -1;
	}
	pr_info("I created %s\n", NODE);
	return 0;
}

static void __exit my_exit(void)
{
	if (my_proc) {
		proc_remove(my_proc);
		pr_info("Removed %s\n", NODE);
	}
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("Jerry Cooperstein");
MODULE_DESCRIPTION("LFD5420:4.13 s_16/lab2_proc.c");
MODULE_LICENSE("GPL v2");
