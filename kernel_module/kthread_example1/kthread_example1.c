

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>

#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/kthread.h>




/*
 * Prevent the kthread exits directly, and make sure when kthread_stop()
 * is called to stop a kthread, it is still alive. If a kthread might be
 * stopped by CACHE_SET_IO_DISABLE bit set, wait_for_kthread_stop() is
 * necessary before the kthread returns.
 */
static inline void wait_for_kthread_stop(void)
{
    pr_debug("wait_for_kthread_stop: 1\n");
	while (!kthread_should_stop()) {

        pr_debug("wait_for_kthread_stop: 2\n");
		set_current_state(TASK_INTERRUPTIBLE);
		schedule();
	}

    pr_debug("wait_for_kthread_stop: 3\n");
}


static int example_thread_run(void *arg)
{
    int i;

    pr_debug("example_thread: 1\n");


    for (i = 0; i < 100 && !kthread_should_stop(); i++)
    {
        pr_debug("example_thread: 2, i: %d\n", i);

        msleep(1000);
    }


    pr_debug("example_thread: 3\n");
    wait_for_kthread_stop();

    pr_debug("example_thread: 4\n");
	return 0;
}



struct task_struct* example_thread;


// module init ----------------------------------------------------------

static int __init kthread_example_init(void)
{
    pr_info("kthread_example_init begin\n");

    example_thread = kthread_run(example_thread_run, NULL, "kthread_example");
    if  (!example_thread)
        return -1;

    pr_info("kthread_example_init end\n");
    return 0;
}

static void __exit kthread_example_exit(void)
{
    pr_info("kthread_example_exit begin\n");

    kthread_stop(example_thread);


    pr_info("kthread_example_exit end\n");
    pr_debug("-------------------------------------------------\n");
}


module_init(kthread_example_init);
module_exit(kthread_example_exit);
MODULE_LICENSE("GPL");
