/* **************** LFD5420:4.13 s_16/lab3_proc_solB.c **************** */
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
my_read(struct file *file, char __user *buf, size_t lbuf, loff_t *ppos)
{
	char *d_iname;
	int nbytes;
	d_iname = file->f_path.dentry->d_iname;
	if (!strncmp(d_iname, NODE_1, strlen(NODE_1))) {
		pr_info("I am reading = %s to %d\n", d_iname, param_1);
		nbytes = sprintf(kstring, "%d\n", param_1);
	} else {
		pr_info("I am reading = %s to %d\n", d_iname, param_2);
		nbytes = sprintf(kstring, "%d\n", param_2);
	}
	return simple_read_from_buffer(buf, lbuf, ppos, kstring, nbytes);
}

static ssize_t my_write(struct file *file, const char __user *buf, size_t lbuf,
			loff_t *ppos)
{
	ssize_t rc;
	char *d_iname;
	d_iname = file->f_path.dentry->d_iname;
	rc = simple_write_to_buffer(kstring, lbuf, ppos, buf, lbuf);
	if (!strncmp(d_iname, NODE_1, strlen(NODE_1))) {
		sscanf(kstring, "%d", &param_1);
		pr_info("I am setting = %s to %d\n", d_iname, param_1);
	} else {
		sscanf(kstring, "%d", &param_2);
		pr_info("I am setting = %s to %d\n", d_iname, param_2);
	}
	return rc;
}

static const struct file_operations my_proc_fops = {
	.owner = THIS_MODULE,
	.read = my_read,
	.write = my_write,
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
				&my_proc_fops);
	if (!my_proc_1) {
		pr_err("I failed to make %s\n", NODE_1);
		proc_remove(my_proc_dir);
		return -1;
	}
	pr_info("I created %s\n", NODE_1);

	my_proc_2 = proc_create(NODE_2, S_IRUGO | S_IWUSR, my_proc_dir,
				&my_proc_fops);

	if (!my_proc_2) {
		pr_err("I failed to make %s\n", NODE_2);
		proc_remove(my_proc_1);
		proc_remove(my_proc_dir);
		remove_proc_entry(NODE_DIR, NULL);
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
MODULE_DESCRIPTION("LFD5420:4.13 s_16/lab3_proc_solB.c");
MODULE_LICENSE("GPL v2");
