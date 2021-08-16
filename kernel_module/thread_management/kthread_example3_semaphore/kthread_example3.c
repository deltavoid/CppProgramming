

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>

#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/semaphore.h>



struct semaphore sem_exit;




struct example_thread_ctx {


    // int finished;
    // struct wait_queue_head wq_head;

    struct semaphore* sem_exit_p;

};

static int example_thread_ctx_init(struct example_thread_ctx* ctx, struct semaphore* sem_exit_p)
{
    // ctx->finished = 0;
    // init_waitqueue_head(&ctx->wq_head);
    ctx->sem_exit_p = sem_exit_p;
    return 0;
}


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
    struct example_thread_ctx* ctx = (struct example_thread_ctx*)arg;

    pr_debug("example_thread: 1\n");


    for (i = 0; i < 10 && !kthread_should_stop(); i++)
    {
        pr_debug("example_thread: 2, i: %d\n", i);

        msleep(1000);
    }


    // ctx->finished = 1;
    // smp_store_release(&ctx->finished, 1);

    up(ctx->sem_exit_p);




    pr_debug("example_thread: 3\n");
    wait_for_kthread_stop();

    pr_debug("example_thread: 4\n");
    for (i = 0; i < 10; i++)
    {
        pr_debug("example_thread: 4.1, i: %d\n", i);
        msleep(1000);
    }
    
    pr_debug("example_thread: 5\n");
	return 0;
}


#define NR_THREADS 4
struct task_struct* example_threads[NR_THREADS];
struct example_thread_ctx example_thread_ctxs[NR_THREADS];


// module init ----------------------------------------------------------

static int __init kthread_example1_init(void)
{
    int i;
    pr_info("kthread_example1_init begin\n");

    sema_init(&sem_exit, 0);


    for (i = 0; i < NR_THREADS; i++)
        example_thread_ctx_init(&example_thread_ctxs[i], &sem_exit);

    for (i = 0; i < NR_THREADS; i++)
    {
        example_threads[i] = kthread_run(example_thread_run, &example_thread_ctxs[i], "kthread_example");
        if  (!example_threads[i])
            return -1;
    }
    

    // while (example_thread_ctx.finished == 0)
    // while (smp_load_acquire(&example_thread_ctx.finished) == 0)
    // {
    //     cond_resched();
    //     cpu_relax();
    // }

    for (i = 0; i < NR_THREADS; i++)
    {
        while (down_interruptible(&sem_exit) < 0)
            pr_debug("Interrupted during semaphore wait\n");
    }
    


    pr_info("kthread_example1_init end\n");
    return 0;
}

static void __exit kthread_example1_exit(void)
{
    int i;
    pr_info("kthread_example1_exit begin\n");

    for (i = 0; i < NR_THREADS; i++)
    {   kthread_stop(example_threads[i]);
    }
    


    pr_info("kthread_example1_exit end\n");
    pr_debug("-------------------------------------------------\n");
}


module_init(kthread_example1_init);
module_exit(kthread_example1_exit);
MODULE_LICENSE("GPL");
