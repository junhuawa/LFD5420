/* **************** LFD5420:4.13 s_16/lab4_proc_sig.c **************** */
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
 * Using proc to send signals.
 *
 * It is sometimes desirable to send a signal to an application from
 * within the kernel.  The function for doing this is:
 *
 * int send_sig(int signal, struct task_struct *tsk, int priv);
 *
 * where signal is the signal to send, tsk points to the task
 * structure corresponding to the process to which the signal should be
 * sent, and priv indicates the privilege level (0 for user applications,
 * 1 for the kernel.)
 *
 * Write a module that opens up two entries in the proc file system.

 *    When the first entry is written to, it sets the process ID of the
 *           process which is registered to receive signals via this mechanism.
 *    When the second entry is written to, it gets the signal to be
 *           delivered and then sends it.
 *    Reading either entry simply shows the current values of these
 *           parameters.
 *
 * Remember you'll have to use pid_task(find_vpid PIDTYPE_PID) to
 * connect the pid to the task structure it corresponds with.
 @*/

#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/init.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/version.h>
/* For changes in signal handler action struction location */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,11,0)
#include <linux/sched/signal.h>
#endif


#define NODE_DIR "my_sig_dir"
#define NODE_1 "pid"
#define NODE_2 "signal"

static int sig_pid = -1, sig_tosend = SIGUSR1;
static struct task_struct *sig_tsk;
static struct proc_dir_entry *proc_sigdir, *proc_pid, *proc_signal;

#define KS 32
static char kstring[KS];	/* should be less sloppy about overflows :) */

static ssize_t
my_read_pid(struct file *file, char __user *buf, size_t lbuf, loff_t *ppos)
{
	int nbytes = sprintf(kstring, "%d\n", sig_pid);
	pr_info("I am reading sig_pid = %d\n", sig_pid);
	return simple_read_from_buffer(buf, lbuf, ppos, kstring, nbytes);
}

static ssize_t
my_read_signal(struct file *file, char __user *buf, size_t lbuf, loff_t *ppos)
{
	int nbytes = sprintf(kstring, "%d\n", sig_tosend);
	pr_info("I am reading sig_tosend = %d\n", sig_tosend);
	return simple_read_from_buffer(buf, lbuf, ppos, kstring, nbytes);
}

static ssize_t my_write_pid(struct file *file, const char __user *buf,
			    size_t lbuf, loff_t *ppos)
{
	ssize_t rc = simple_write_to_buffer(kstring, lbuf, ppos, buf, lbuf);
	sscanf(kstring, "%d", &sig_pid);
	pr_info("I am setting sig_pid %d\n", sig_pid);
	sig_tsk = pid_task(find_vpid(sig_pid), PIDTYPE_PID);
	return rc;
}

static ssize_t my_write_signal(struct file *file, const char __user *buf,
			       size_t lbuf, loff_t *ppos)
{
	int retval;
	ssize_t rc = simple_write_to_buffer(kstring, lbuf, ppos, buf, lbuf);
	sscanf(kstring, "%d", &sig_tosend);
	pr_info("I am setting sig_tosend %d\n", sig_tosend);
	if (!sig_tsk) {
		pr_info("You haven't set the pid; using current\n");
		sig_tsk = current;
		sig_pid = (int)current->pid;
	}
	pr_info(" Sending signal %d to process ID %d\n", sig_tosend, sig_pid);
	retval = send_sig(sig_tosend, sig_tsk, 0);
	pr_info("retval = %d\n", retval);
	return rc;
}

static const struct file_operations proc_fops_pid = {
	.owner = THIS_MODULE,
	.read = my_read_pid,
	.write = my_write_pid,
};

static const struct file_operations proc_fops_signal = {
	.owner = THIS_MODULE,
	.read = my_read_signal,
	.write = my_write_signal,
};

static int __init my_init(void)
{
	proc_sigdir = proc_mkdir(NODE_DIR, NULL);
	if (!proc_sigdir) {
		pr_err("I failed to make %s\n", NODE_DIR);
		return -1;
	}
	pr_info("I created %s\n", NODE_DIR);

	proc_pid = proc_create(NODE_1, S_IRUGO | S_IWUSR, proc_sigdir,
			       &proc_fops_pid);
	if (!proc_pid) {
		pr_err("I failed to make %s\n", NODE_1);
		proc_remove(proc_sigdir);
		return -1;
	}
	pr_info("I created %s\n", NODE_1);

	proc_signal = proc_create(NODE_2, S_IRUGO | S_IWUSR, proc_sigdir,
				  &proc_fops_signal);

	if (!proc_signal) {
		pr_err("I failed to make %s\n", NODE_2);
		proc_remove(proc_pid);
		proc_remove(proc_sigdir);
		return -1;
	}
	pr_info("I created %s\n", NODE_2);
	return 0;
}

static void __exit my_exit(void)
{
	if (proc_pid) {
		proc_remove(proc_pid);
		pr_info("Removed %s\n", NODE_1);
	}
	if (proc_signal) {
		proc_remove(proc_signal);
		remove_proc_entry(NODE_2, proc_sigdir);
		pr_info("Removed %s\n", NODE_2);
	}
	if (proc_sigdir) {
		proc_remove(proc_sigdir);
		pr_info("Removed %s\n", NODE_DIR);
	}
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("Jerry Cooperstein");
MODULE_DESCRIPTION("LFD5420:4.13 s_16/lab4_proc_sig.c");
MODULE_LICENSE("GPL v2");
