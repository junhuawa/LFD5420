/* **************** LFD5420:4.13 s_16/lab3_proc_solA.c **************** */
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
 * Making your own subdirectory in proc.
 *
 * Write a module that creates your own proc filesystem subdirectory
 * and creates at least two entries under it.

 * As in the first exercise, reading an entry should obtain a
 * parameter value, and writing it should reset it.

 * You may use the data element in the proc_dir_entry structure to use
 * the same callback functions for multiple entries.
 @*/

#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/init.h>

#define NODE_DIR "my_proc_dir"
#define NODE_1 "param_1"
#define NODE_2 "param_2"

static int param_1 = 100, param_2 = 200;
static struct proc_dir_entry *my_proc_dir, *my_proc_1, *my_proc_2;

#define KS 32
static char kstring[KS];	/* should be less sloppy about overflows :) */
static ssize_t
my_read1(struct file *file, char __user *buf, size_t lbuf, loff_t *ppos)
{
	int nbytes = sprintf(kstring, "%d\n", param_1);
	pr_info("I am reading param_1 = %d\n", param_1);
	return simple_read_from_buffer(buf, lbuf, ppos, kstring, nbytes);
}

static ssize_t
my_read2(struct file *file, char __user *buf, size_t lbuf, loff_t *ppos)
{
	int nbytes = sprintf(kstring, "%d\n", param_2);
	pr_info("I am reading param_1 = %d\n", param_2);
	return simple_read_from_buffer(buf, lbuf, ppos, kstring, nbytes);
}

static ssize_t my_write1(struct file *file, const char __user *buf,
			 size_t lbuf, loff_t *ppos)
{
	ssize_t rc = simple_write_to_buffer(kstring, lbuf, ppos, buf, lbuf);
	sscanf(kstring, "%d", &param_1);
	pr_info("I am setting param_1 to %d\n", param_1);
	return rc;
}

static ssize_t my_write2(struct file *file, const char __user *buf,
			 size_t lbuf, loff_t *ppos)
{
	ssize_t rc = simple_write_to_buffer(kstring, lbuf, ppos, buf, lbuf);
	sscanf(kstring, "%d", &param_2);
	pr_info("I am setting param_2 to %d\n", param_2);
	return rc;
}

static const struct file_operations my_proc_fops1 = {
	.owner = THIS_MODULE,
	.read = my_read1,
	.write = my_write1,
};

static const struct file_operations my_proc_fops2 = {
	.owner = THIS_MODULE,
	.read = my_read2,
	.write = my_write2,
};

static int __init my_init(void)
{
	my_proc_dir = proc_mkdir(NODE_DIR, NULL);
	if (!my_proc_dir) {
		pr_err("I failed to make %s\n", NODE_DIR);
		return -1;
	}
	pr_info("I created %s\n", NODE_DIR);

	my_proc_1 = proc_create(NODE_1, S_IRUGO | S_IWUSR, my_proc_dir,
				&my_proc_fops1);
	if (!my_proc_1) {
		pr_err("I failed to make %s\n", NODE_1);
		proc_remove(my_proc_dir);
		return -1;
	}
	pr_info("I created %s\n", NODE_1);

	my_proc_2 = proc_create(NODE_2, S_IRUGO | S_IWUSR, my_proc_dir,
				&my_proc_fops2);

	if (!my_proc_2) {
		pr_err("I failed to make %s\n", NODE_2);
		proc_remove(my_proc_1);
		proc_remove(my_proc_dir);
		return -1;
	}
	pr_info("I created %s\n", NODE_2);
	return 0;
}

static void __exit my_exit(void)
{
	if (my_proc_1) {
		proc_remove(my_proc_1);
		pr_info("Removed %s\n", NODE_1);
	}
	if (my_proc_2) {
		proc_remove(my_proc_2);
		pr_info("Removed %s\n", NODE_2);
	}
	if (my_proc_dir) {
		proc_remove(my_proc_dir);
		pr_info("Removed %s\n", NODE_DIR);
	}
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("Jerry Cooperstein");
MODULE_DESCRIPTION("LFD5420:4.13 s_16/lab3_proc_solA.c");
MODULE_LICENSE("GPL v2");
