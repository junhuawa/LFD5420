/* **************** LFD5420:4.13 s_16/lab5_seqfile.c **************** */
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
 * Using seq_file for the proc filesystem.
 *
 * Take the simple "x_busy" proc entry discussed earlier, and
 * re-implement it using the seq_file interface.
 *
 * As a parameter, input the number of lines to print out.
 *
 @*/

#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/init.h>
#include <linux/seq_file.h>

static int items = 1, x_delay = 1;
static unsigned long future;
static struct proc_dir_entry *x_proc_busy;

#define NODE "x_busy"

/* Sequential file iterator                                              */

static void *x_busy_seq_start(struct seq_file *sf, loff_t *pos)
{
	if (*pos < items) {
		future = jiffies + x_delay * HZ;
		while (time_before(jiffies, future))/* nothing */ ;
		return (void *)&jiffies;
	}
	return NULL;
}

static void *x_busy_seq_next(struct seq_file *sf, void *v, loff_t *pos)
{
	(*pos)++;
	if (*pos < items) {
		future = jiffies + x_delay * HZ;
		while (time_before(jiffies, future))/* nothing */ ;
		return (void *)&jiffies;
	}
	return NULL;
}

static void x_busy_seq_stop(struct seq_file *sf, void *v)
{
	/* Nothing to do here */
}

static int x_busy_seq_show(struct seq_file *sf, void *v)
{
	volatile unsigned long *const jp = (volatile unsigned long *)v;
	seq_printf(sf, "jiffies = %lu.\n", *jp);
	return 0;
}

static const struct seq_operations proc_x_busy_seq_ops = {
	.start = x_busy_seq_start,
	.next = x_busy_seq_next,
	.stop = x_busy_seq_stop,
	.show = x_busy_seq_show,
};

static int proc_x_busy_open(struct inode *inode, struct file *file)
{
	return seq_open(file, &proc_x_busy_seq_ops);
}

static const struct file_operations proc_x_busy_operations = {
	.open = proc_x_busy_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = seq_release
};

static int __init my_init(void)
{
	x_proc_busy = proc_create(NODE, S_IRUGO | S_IWUSR, NULL,
				  &proc_x_busy_operations);
	if (!x_proc_busy)
		return -1;
	return 0;
}

static void __exit my_exit(void)
{
	if (x_proc_busy)
		proc_remove(x_proc_busy);
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("Jerry Cooperstein");
MODULE_DESCRIPTION("LFD5420:4.13 s_16/lab5_seqfile.c");
MODULE_LICENSE("GPL v2");
module_param(items, int, S_IRUGO);
MODULE_PARM_DESC(items, "How many items to simulate");
