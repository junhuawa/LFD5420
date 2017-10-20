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
#include <linux/slab.h>
#include <linux/notifier.h>
#include <linux/usb.h>

struct notifier_block usb_notifier_block;

int usb_notifier_call(struct notifier_block *block, unsigned long event, void *data)
{
    switch (event) {
        case USB_DEVICE_ADD:
            pr_info("usb add device event trigger!\n");
            return NOTIFY_OK;
            break;

        case USB_DEVICE_REMOVE:
            pr_info("usb remove device event trigger!\n");
            return NOTIFY_OK;
            break;

        default:
            return NOTIFY_OK;
    }
    return NOTIFY_OK;
}

static int __init my_init(void)
{
    memset(&usb_notifier_block, '\0', sizeof(struct notifier_block));
    usb_notifier_block.notifier_call = usb_notifier_call;
    usb_register_notify(&usb_notifier_block);
	pr_info("Hello: module loaded at 0x%p\n", my_init);
	return 0;
}

static void __exit my_exit(void)
{
        usb_unregister_notify(&usb_notifier_block);
        pr_info("Bye: module unloaded from 0x%p\n", my_exit);
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("A GENIUS");
MODULE_LICENSE("GPL v2");
