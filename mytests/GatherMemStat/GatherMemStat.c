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
#include <linux/mm.h>

int locked_pages = 0;
int free_page_counter = 0;

static void walk_all_pages(unsigned long num)
{
	unsigned long i;
	struct page *page_ptr;
	for(i=0; i<num; i++) {

		if(!pfn_valid(i)) continue;
		page_ptr = pfn_to_page(i); 
		if(PageLocked(page_ptr)) { 
			locked_pages ++;
		}
		if(page_count(page_ptr) == 0) {
			free_page_counter ++;
		}
	}

	pr_info("locked pages number: %d\n", locked_pages);
	pr_info("free pages number: %d\n", free_page_counter);
}

static int __init my_init(void)
{

	unsigned long num_physpages;

	num_physpages = get_num_physpages();
	walk_all_pages(num_physpages);

	pr_info("page size: %ld, num of physical pages: %lu, TASK_SIZE: 0x%x\n", PAGE_SIZE, num_physpages, TASK_SIZE);
	pr_info("Hello: module loaded at 0x%p\n", my_init);
	return 0;
}

static void __exit my_exit(void)
{
        pr_info("Bye: module unloaded from 0x%p\n", my_exit);
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("A GENIUS");
MODULE_LICENSE("GPL v2");
