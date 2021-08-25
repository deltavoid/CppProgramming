

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>

#include <linux/tracepoint.h>
#include <trace/events/sched.h>




// preempt_count display ------------------------------------------


static void task_struct_display(const char* prefix, struct task_struct* task_p)
{
    // preempt_disable();
    // comm should use get_task_comm
    pr_debug("%s task tid/pid: %d, pid/tgid: %d, comm: %s\n", 
            prefix, task_p->pid, task_p->tgid, task_p->comm);
    // preempt_enable();
}

static void current_display(void)
{
    struct task_struct* task_p = current;

    preempt_disable();
    // comm should use get_task_comm
    pr_debug("current: cpu_id: %d, task tid/pid: %d, pid/tgid: %d, comm: %s\n", 
            smp_processor_id(), task_p->pid, task_p->tgid, task_p->comm);
    preempt_enable();

    // task_struct_display("current:", current);

    pr_debug("preempt_count: 0x%08x, test_preempt_need_resched: %d\n", preempt_count(), test_preempt_need_resched());
    // pr_debug("test_preempt_need_resched: %d\n", test_preempt_need_resched());
}



// probe sched ----------------------------------------------------

#define CPU_ID 0

// u64 __percpu * probe_sched_wakeup_count;

// static void probe_sched_wakeup(void *priv, struct task_struct *p)
// {
//     u64* count = this_cpu_ptr(probe_sched_wakeup_count);
    
//     // if  (smp_processor_id() == CPU_ID && ++*count % 1000 == 0)
//     if  (smp_processor_id() == CPU_ID)
//     {
//         pr_debug("probe_sched_wakeup: \n");
//         // preempt_count_display();
//         current_display();

//         task_struct_display("target: ", p);

//         pr_debug("\n");
//     }
// }


// u64 __percpu * probe_sched_switch_count;

static void trace_sched_switch_probe(void *priv, 
        bool preempt, struct task_struct *prev, struct task_struct *next)
{
    // u64* count = this_cpu_ptr(probe_sched_switch_count);
    
    // if  (smp_processor_id() == CPU_ID && ++*count % 1000 == 0)
    if  (smp_processor_id() == CPU_ID)
    {
        pr_debug("trace_sched_switch_probe: cpu_id: %d, preempt: %d\n", smp_processor_id(), preempt);
        // preempt_count_display();
        // current_display();

        task_struct_display("prev: ", prev);
        task_struct_display("next: ", next);

        pr_debug("\n");
    }

}


// u64 __percpu * probe_sched_migrate_count;

// static void probe_sched_migrate_task(void *priv, struct task_struct *p, int cpu)
// {
//     u64* count = this_cpu_ptr(probe_sched_migrate_count);
    
//     // if  (smp_processor_id() == CPU_ID && ++*count % 10 == 0)
//     if  (smp_processor_id() == CPU_ID)
//     {
//         pr_debug("probe_sched_migrate: target cpu: %d\n", cpu);
//         // preempt_count_display();
//         current_display();

//         task_struct_display("p: ", p);

//         pr_debug("\n");
//     }
// }


static void trace_sched_stat_sleep_probe(void* priv,
        struct task_struct *tsk, u64 delay)
{
    if  (smp_processor_id() == CPU_ID)
    {
        pr_debug("trace_sched_stat_sleep_probe: cpu_id: %d, delay: %llu\n", smp_processor_id(), delay);

        task_struct_display("task: ", tsk);

        pr_debug("\n");
    }

}

// tracepoint_probe_context ----------------------------------------

struct tracepoint_probe_entry {
    char* name;
    struct tracepoint *tp;
    void* probe;
    void* priv;
};

#define TRACEPOINT_PROBE_CONTEXT_MAX_NUM 8

struct tracepoint_probe_context {
    struct tracepoint_probe_entry entries[TRACEPOINT_PROBE_CONTEXT_MAX_NUM];
    int init_num;
    int found_num;
};

static void tracepoint_lookup(struct tracepoint *tp, void *priv)
{
    int i;
    struct tracepoint_probe_context* ctx = priv;

    if  (ctx->found_num == ctx->init_num)
        return;

    for (i = 0; i < ctx->init_num; i++)
    {   struct tracepoint_probe_entry* entry = &ctx->entries[i];

        if  (entry->tp || strcmp(tp->name, entry->name))
            continue;
        entry->tp = tp;
        ctx->found_num++;
    }
}

static int tracepoint_probe_context_find_tracepoints(struct tracepoint_probe_context* ctx)
{
    int i;
    for (i = 0; i < ctx->init_num; i++)
        ctx->entries[i].tp = NULL;
    ctx->found_num = 0;


    for_each_kernel_tracepoint(tracepoint_lookup, ctx);

    if  (ctx->found_num != ctx->init_num)
        return -1;
    return 0;
}

static int tracepoint_probe_context_register_probes(struct tracepoint_probe_context* ctx)
{
    int i;

    for (i = 0; i < ctx->init_num; i++)
    {   struct tracepoint_probe_entry* entry = &ctx->entries[i];
        int ret = tracepoint_probe_register(entry->tp, entry->probe, entry->priv);
        if  (ret)
            pr_warn("trace_%s probe failed\n", entry->name);

    }

    return 0;
}

static void tracepoint_probe_context_unregister_probes(struct tracepoint_probe_context* ctx)
{
    int i;

    for (i = 0; i < ctx->init_num; i++)
    {   struct tracepoint_probe_entry* entry = &ctx->entries[i];
        tracepoint_probe_unregister(entry->tp, entry->probe, entry->priv);
    }

    tracepoint_synchronize_unregister();
}


// module init -----------------------------------------------------

static struct tracepoint_probe_context sched_probes = {
    .entries = {
        // {
        //     .name = "sched_wakeup",
        //     .probe = probe_sched_wakeup,
        //     .priv = NULL,
        // },
        // {
        //     .name = "sched_switch",
        //     .probe = trace_sched_switch_probe,
        //     .priv = NULL,
        // },
        // {
        //     .name = "sched_migrate_task",
        //     .probe = probe_sched_migrate_task,
        //     .priv = NULL,
        // },
        {
            .name = "sched_stat_sleep",
            .probe = trace_sched_stat_sleep_probe,
            .priv = NULL,
        },

    },
    .init_num = 1
};



static int __init tracepoint_init(void)
{
    pr_debug("tracepoint_init begin\n");

    // probe_sched_wakeup_count = alloc_percpu(u64);
    // probe_sched_switch_count = alloc_percpu(u64);
    // probe_sched_migrate_count = alloc_percpu(u64);

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
    // free_percpu(probe_sched_switch_count);
    // free_percpu(probe_sched_migrate_count);

    pr_debug("tracepoint_exit end\n");
    pr_debug("------------------------------------------------------------------\n");
}

module_init(tracepoint_init)
module_exit(tracepoint_exit)
MODULE_LICENSE("GPL");
