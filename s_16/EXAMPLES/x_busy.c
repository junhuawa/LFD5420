/* **************** LFD5420:4.13 s_16/x_busy.c **************** */
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
#include <linux/proc_fs.h>

static struct proc_dir_entry *x_proc_busy;
static char kstring[32];        /* should be less sloppy about overflows :) */

static int x_delay = 1;         /* the default delay */
static ssize_t
x_busy_read(struct file *file, char __user * buf, size_t lbuf, loff_t * ppos)
{
        int nbytes;
        unsigned long j = jiffies + x_delay * HZ;
        while (time_before(jiffies, j))
                /* nothing */ ;
        nbytes = sprintf(kstring, "%ld\n", jiffies);
        return simple_read_from_buffer(buf, lbuf, ppos, kstring, nbytes);
}

static const struct file_operations xbusy_fops = {
        .owner = THIS_MODULE,
        .read = x_busy_read,
};

static int __init my_init(void)
{
        x_proc_busy =
            proc_create("x_busy", S_IRUGO | S_IWUSR, NULL, &xbusy_fops);
        return 0;
}

static void __exit my_exit(void)
{
        if (x_proc_busy)
		proc_remove(x_proc_busy);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL v2");
