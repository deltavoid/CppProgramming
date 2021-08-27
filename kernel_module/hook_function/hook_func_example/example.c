

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>

#include "hook_func_lib.h"
#include <trace/events/sched.h>





// preempt_count display ------------------------------------------

static void preempt_count_display(void)
{
    struct task_struct* task_p = current;

    preempt_disable();
    // comm should use get_task_comm
    pr_debug("cpu_id: %d, task tid/pid: %d, pid/tgid: %d, comm: %s\n", 
            smp_processor_id(), task_p->pid, task_p->tgid, task_p->comm);
    preempt_enable();

    pr_debug("preempt_count: 0x%08x, test_preempt_need_resched: %d\n", preempt_count(), test_preempt_need_resched());
    // pr_debug("test_preempt_need_resched: %d\n", test_preempt_need_resched());
}





static void probe_local_timer_entry(int id)
{
    if  (smp_processor_id() == 0)
    {

        pr_debug("probe_local_timer_entry, jiffies: %ld\n", jiffies);
        preempt_count_display();

        dump_stack();

        printk("\n");
    }

}



// module init -----------------------------------------------------

static struct tracepoint_probe_context sched_probes = {
    .entries = {

        {
            .name = "local_timer_entry",
            .probe = probe_local_timer_entry,
            .priv = NULL,
        },
    },
    .init_num = 1
};



static int __init tracepoint_init(void)
{
    pr_debug("tracepoint_init begin\n");

    // probe_sched_wakeup_count = alloc_percpu(u64);

    if  (tracepoint_probe_context_find_tracepoints(&sched_probes) < 0)
    {   pr_warn("find tracepoints failed\n");
        return -1;
    }

    if  (tracepoint_probe_context_register_probes(&sched_probes) < 0)
    {   pr_warn("register probes failed\n");
        return -1;
    }


    pr_debug("tracepoint_init end\n");
    return 0;
}

static void __exit tracepoint_exit(void)
{
    pr_debug("tracepoint_exit begin\n");

    tracepoint_probe_context_unregister_probes(&sched_probes);

    // free_percpu(probe_sched_wakeup_count);

    pr_debug("tracepoint_exit end\n");
    pr_debug("------------------------------------------------------------------\n");
}

module_init(tracepoint_init)
module_exit(tracepoint_exit)
MODULE_LICENSE("GPL");
