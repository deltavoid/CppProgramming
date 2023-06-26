

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>

#include <linux/sched.h>
#include <uapi/linux/sched/types.h>
#include <linux/delay.h>
#include <linux/kthread.h>





// for arm
uint64_t qjl_gettick10ns(void)
{    
    uint64_t count_num;    
    __asm__ __volatile__ (
        "mrs %0, cntvct_el0" 
        : "=r" (count_num)
    );    
    
    return count_num;
}

uint64_t simple_arm_clock(void)
{
    return qjl_gettick10ns() * 10;
}





struct example_thread_ctx {
    struct task_struct* task;

    int running;
    int waked;
    struct wait_queue_head wq_head;

    uint64_t t0;
    uint64_t t1;
};

static int example_thread_ctx_init(struct example_thread_ctx* ctx)
{
    ctx->running = 1;
    ctx->waked = 0;
    init_waitqueue_head(&ctx->wq_head);
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
    int i = 0;
    struct example_thread_ctx* thread_ctx = (struct example_thread_ctx*)arg;

    pr_debug("example_thread: 1\n");


    // for (i = 0; i < 10 && !kthread_should_stop(); i++)
    // {
    //     pr_debug("example_thread: 2, i: %d\n", i);

    //     msleep(1000);
    // }


    // ctx->finished = 1;
    // wake_up(&ctx->wq_head);



    // pr_debug("example_thread: 3\n");
    // wait_for_kthread_stop();

    // pr_debug("example_thread: 4\n");
    // for (i = 0; i < 10; i++)
    // {
    //     pr_debug("example_thread: 4.1, i: %d\n", i);
    //     msleep(1000);
    // }

    // while (!kthread_should_stop())
    // {
    //     pr_debug("example_thread: 2, i: %d\n", ++i);
    //     wait_event(thread_ctx->wq_head, thread_ctx->waked == 1);
    //     thread_ctx->waked = 0;


    //     // pr_debug("example_thread: 3");
    //     // msleep(1000);
    // }


    while (thread_ctx->running)
    {
        // pr_debug("example_thread: 2, i: %d\n", ++i);
        wait_event(thread_ctx->wq_head, thread_ctx->waked == 1);
        thread_ctx->waked = 0;
        thread_ctx->t1 = simple_arm_clock();

        pr_info("t0: %llu, t1: %llu, duration: %llu",  thread_ctx->t0,  thread_ctx->t1,  
                thread_ctx->t1 -  thread_ctx->t0);


        // pr_debug("example_thread: 3");
        // msleep(1000);
    }


    wait_for_kthread_stop();
    pr_debug("example_thread: 5\n");
	return 0;
}



// ------------------------------------------------




struct timer_example {

    struct example_thread_ctx* thread_ctx;


    struct timer_list timer;
    uint64_t cnt;
};

#define timer_example_timeout 5

static void timer_example_expire__(struct timer_example* timer_ctx, struct timer_list* timer)
{
    pr_debug("timer_example_expire__, cnt: %lld\n", ++timer_ctx->cnt);
    // preempt_count_display();
    // dump_stack();

    // if  (++data->cnt == 10)
    // {   pr_debug("del_timer\n");
    //     del_timer(timer);
    // }
    // else
    // {   pr_debug("jeffies: %ld\n", jiffies);
    //     mod_timer(timer, jiffies + timer_example_timeout * HZ);
    // }


    timer_ctx->thread_ctx->t0 = simple_arm_clock();
    timer_ctx->thread_ctx->waked = 1;
    wake_up(&timer_ctx->thread_ctx->wq_head);

    // pr_debug("jeffies: %ld\n", jiffies);
    mod_timer(timer, jiffies + timer_example_timeout * HZ);
    // printk("\n");
}

static void timer_example_expire(struct timer_list *timer)
{
    struct timer_example* data = from_timer(data, timer, timer);
    timer_example_expire__(data, timer);
}

static int timer_example_init(struct timer_example* data)
{
    data->cnt = 0;
    timer_setup(&data->timer, timer_example_expire, 0);
    mod_timer(&data->timer, jiffies + timer_example_timeout * HZ);
    return 0;
}

static void timer_example_exit(struct timer_example* data)
{
    pr_debug("timer_example_exit, timer_pending: %d\n", timer_pending(&data->timer));
    if  (timer_pending(&data->timer))
    {   pr_debug("timer_example_exit, del_timer\n");
        del_timer(&data->timer);

        // del_timer_sync(&data->timer); // can not be used in atomic context
    }
}

















struct task_struct* example_thread;
struct example_thread_ctx example_thread_ctx;
struct timer_example example_timer;


// module init ----------------------------------------------------------

static int __init kthread_example4_init(void)
{
    struct sched_param thread_param = {
		.sched_priority = MAX_USER_RT_PRIO/2,
	};

    pr_info("kthread_example4_init begin\n");

    example_thread_ctx_init(&example_thread_ctx);

    example_thread = kthread_run(example_thread_run, &example_thread_ctx, "kthread_example4");
    if  (!example_thread)
        return -1;

    example_thread_ctx.task = example_thread;


    example_timer.thread_ctx = &example_thread_ctx;


    sched_setscheduler_nocheck(example_thread_ctx.task, SCHED_FIFO, &thread_param);    


    timer_example_init(&example_timer);
    



    // wait_event(example_thread_ctx.wq_head, example_thread_ctx.finished == 1);


    pr_info("kthread_example4_init end\n");
    return 0;
}

static void __exit kthread_example4_exit(void)
{
    pr_info("kthread_example4_exit begin\n");

    timer_example_exit(&example_timer);

    example_thread_ctx.running = 0;
    example_thread_ctx.waked = 1;
    wake_up(&example_thread_ctx.wq_head);

    kthread_stop(example_thread);

    pr_info("kthread_example4_exit end\n");
    pr_debug("-------------------------------------------------\n");
}


module_init(kthread_example4_init);
module_exit(kthread_example4_exit);
MODULE_LICENSE("GPL");
