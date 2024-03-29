

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>

#include <linux/kprobes.h>



// preempt_count_display --------------------------------------------



static void preempt_count_display(void)
{
    pr_debug("preempt_count: 0x%08x\n", preempt_count());
    pr_debug("test_preempt_need_resched: %d\n", test_preempt_need_resched());
    
    pr_debug("hardirq_count: %ld\n", hardirq_count());
    pr_debug("softirq_count: %ld\n", softirq_count());
    pr_debug("irq_count: %ld\n", irq_count());
    
    pr_debug("in_irq: %ld\n", in_irq());
    pr_debug("in_serving_softirq: %ld\n", in_serving_softirq());
    pr_debug("in_nmi: %ld\n", in_nmi());
    pr_debug("in_task: %d\n", in_task());

}

static void thread_display(void)
{
    struct thread_info* thread_p = current_thread_info();
    struct task_struct* task_p = current;

    pr_debug("thread_p: 0x%px, task_p: 0x%px\n", thread_p, task_p);
    
    
    // comm should use get_task_comm
    pr_debug("cpu_id: %d, task tid/pid: %d, pid/tgid: %d, comm: %s\n", 
            smp_processor_id(), task_p->pid, task_p->tgid, task_p->comm);

    pr_debug("thread flags: 0x%lx\n", thread_p->flags);
    pr_debug("thread status: 0x%x\n", thread_p->status);

    pr_debug("task flags: 0x%x\n", task_p->flags);
    pr_debug("task state: 0x%lx\n", task_p->state);


}

static void current_display(void)
{
    pr_debug("current_display: 1\n");
    thread_display();

    pr_debug("current_display: 2\n");
    preempt_count_display();

    pr_debug("current_display: 3\n");
    pr_debug("\n");
}



// kprobe -----------------------------------------------------------

#define MAX_SYMBOL_LEN	64
static char symbol[MAX_SYMBOL_LEN] = "_do_fork";
module_param_string(symbol, symbol, sizeof(symbol), 0644);



/* For each probe you need to allocate a kprobe structure */
static struct kprobe kp = {
    .symbol_name	= symbol,
};

/* for any context, information need to know
 * cpu, tid, pid, command name
 * preempt_count: thread context, irq context, hard_irq, soft_irq, preempt_disable, 
 *     need_resched
 * lock
 */


static unsigned long cnt = 0;

/* kprobe pre_handler: called just before the probed instruction is executed */
static int handler_pre(struct kprobe *p, struct pt_regs *regs)
{
    if  (++cnt % 100 == 0)
    {
        pr_debug("%s cnt: %ld --------------------------------------------------------------\n", 
                p->symbol_name, cnt);
        // pr_debug("cnt: %ld\n", cnt);
        // preempt_count_display();
        current_display();
        
        dump_stack();
    }
    

    return 0;
}

/*
 * fault_handler: this is called if an exception is generated for any
 * instruction within the pre- or post-handler, or when Kprobes
 * single-steps the probed instruction.
 */
static int handler_fault(struct kprobe *p, struct pt_regs *regs, int trapnr)
{
    pr_info("fault_handler: p->addr = 0x%p, trap #%dn", p->addr, trapnr);
    /* Return 0 because we don't handle the fault. */
    return 0;
}

// preempt_count_display -----------------------------------------------------


spinlock_t example_lock;
static void preempt_count_test(void)
{
    pr_debug("\n");
    pr_debug("preempt_count_test\n");
    preempt_count_display();


    pr_debug("\n");
    spin_lock(&example_lock);

    pr_debug("spin_lock\n");
    preempt_count_display();

    spin_unlock(&example_lock);


    pr_debug("\n");
    local_bh_disable();

    pr_debug("local_bh_disable\n");
    preempt_count_display();

    local_bh_enable();


    pr_debug("\n");
    local_bh_disable();
    local_bh_disable();

    pr_debug("local_bh_disable * 2\n");
    preempt_count_display();

    local_bh_enable();
    local_bh_enable();

    pr_debug("\n");
    local_bh_disable();
    local_bh_disable();
    local_bh_disable();

    pr_debug("local_bh_disable * 3\n");
    preempt_count_display();

    local_bh_enable();
    local_bh_enable();
    local_bh_enable();

    pr_debug("\n");
    spin_lock_bh(&example_lock);

    pr_debug("spin_lock_bh\n");
    preempt_count_display();

    spin_unlock_bh(&example_lock);


    pr_debug("\n");
    preempt_disable();

    pr_debug("preempt_disable\n");
    preempt_count_display();

    preempt_enable();


}


static void dump_stack_test(void)
{
    local_irq_disable();

    dump_stack();

    local_irq_enable();
}

// module init ---------------------------------------------------------------




static int __init preempt_count_display_init(void)
{
    int ret;
    kp.pre_handler = handler_pre;
    // kp.post_handler = handler_post;
    kp.post_handler = NULL;
    kp.fault_handler = handler_fault;

    ret = register_kprobe(&kp);
    if (ret < 0) {
        pr_err("register_kprobe failed, returned %d\n", ret);
        return ret;
    }
    pr_info("Planted kprobe at %p\n", kp.addr);

    current_display();

    preempt_count_display();
    preempt_count_test();
    
    // dump_stack_test();

    return 0;
}

static void __exit preempt_count_display_exit(void)
{
    unregister_kprobe(&kp);

    preempt_count_display();


    pr_info("kprobe at %p unregistered\n", kp.addr);
    pr_debug("----------------------------------------------\n");
}

module_init(preempt_count_display_init)
module_exit(preempt_count_display_exit)
MODULE_LICENSE("GPL");
