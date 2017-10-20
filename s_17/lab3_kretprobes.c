/* **************** LFD5420:4.13 s_17/lab3_kretprobes.c **************** */
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
 * Using kretprobes
 *
 * Instrument do_fork() with kretprobes.
 *
 *    Use the struct task_struct pointer suplied in the
 *       struct kretprobe_instance.
 *
 *    Use the private data mechanism just to illustrate how to
 *       keep per-instance data.  Here we keep the start time (that is,
 *       probed function's entry time), and a unique sequence number.
 *
 @*/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kprobes.h>
#include <linux/version.h>

#define PRTSK(t) pr_info("task:  comm=%s, pid=%d\n", t->comm, t->pid);

/* we store one of these in the private data of each instance */
struct instance_data {
	ktime_t start;
	int seq_number;
};

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 2, 0)
static char *name = "do_fork";
#else
static char *name = "_do_fork";
#endif
module_param(name, charp, S_IRUGO);

static atomic_t seq_nr;

static int entry_handler(struct kretprobe_instance *ri, struct pt_regs *regs)
{
	struct instance_data *instd;

	instd = (struct instance_data *)ri->data;
	instd->start = ktime_get();
	instd->seq_number = atomic_inc_return(&seq_nr);
	pr_info("Entering entry_handler, assigning seq_nr %d\n",
		instd->seq_number);
	PRTSK(ri->task);
	return 0;
}

static int handler(struct kretprobe_instance *ri, struct pt_regs *regs)
{
	long elapsed;
	ktime_t end;
	struct instance_data *instd;

	instd = (struct instance_data *)ri->data;

	pr_info("Entering handler, instance seq_nr is %d\n", instd->seq_number);
	end = ktime_get();
	elapsed = ktime_to_ns(ktime_sub(end, instd->start));
	pr_info("Elapsed time = %ld nanosecs\n", elapsed);

	PRTSK(ri->task);
	return 0;
}

static struct kretprobe rp = {
	.handler = handler,
	.entry_handler = entry_handler,
	.data_size = sizeof(struct instance_data),
	.maxactive = 4,
};

static int __init my_init(void)
{
	/* set the handler functions */

	rp.kp.symbol_name = name;

	if (register_kretprobe(&rp)) {
		pr_info("Failed to register retprobe, quitting\n");
		return -1;
	}

	pr_info("Hello: module loaded at 0x%p\n", my_init);

	return 0;
}

static void __exit my_exit(void)
{
	unregister_kretprobe(&rp);
	pr_info("Bye: module unloaded from 0x%p\n", my_exit);
}

module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("Jerry Cooperstein");
MODULE_AUTHOR("Bill Kerr");
MODULE_DESCRIPTION("LFD5420:4.13 s_17/lab3_kretprobes.c");
MODULE_LICENSE("GPL v2");
