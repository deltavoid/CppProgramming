

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>

#include <linux/kprobes.h>
#include <linux/tracepoint.h>
#include <trace/events/sched.h>





// preempt_count display ------------------------------------------


static void task_struct_display(const char* prefix, struct task_struct* task_p)
{

    pr_debug("%s task tid/pid: %d, pid/tgid: %d, comm: %s\n", 
            prefix, task_p->pid, task_p->tgid, task_p->comm);
}

static void current_display(void)
{
    struct task_struct* task_p = current;

    preempt_disable();
    // comm should use get_task_comm
    pr_debug("current:  task tid/pid: %d, pid/tgid: %d, comm: %s, cpu_id: %d\n", 
            task_p->pid, task_p->tgid, task_p->comm, smp_processor_id());
    preempt_enable();

    // task_struct_display("current:", current);

    pr_debug("preempt_count: 0x%08x, test_preempt_need_resched: %d\n", preempt_count(), test_preempt_need_resched());
    // pr_debug("test_preempt_need_resched: %d\n", test_preempt_need_resched());
}



// probe sched ----------------------------------------------------

#define CPU_ID 0



// u64 __percpu * probe_sched_switch_count;

static void trace_sched_switch_handler(void *priv, 
        bool preempt, struct task_struct *prev, struct task_struct *next)
{
    // u64* count = this_cpu_ptr(probe_sched_switch_count);
    
    // if  (smp_processor_id() == CPU_ID && ++*count % 1000 == 0)
    if  (smp_processor_id() == CPU_ID)
    {
        pr_debug("trace_sched_switch_handler: cpu_id: %d, preempt: %d\n", smp_processor_id(), preempt);
        // preempt_count_display();
        // current_display();

        task_struct_display("prev: ", prev);
        task_struct_display("next: ", next);

        pr_debug("\n");
    }

}


static void trace_sched_stat_runtime_handler(void* priv,
        struct task_struct *tsk, u64 runtime, u64 vruntime)
{
    if  (smp_processor_id() == CPU_ID)
    {
        pr_debug("trace_sched_stat_runtime_handler: cpu_id: %d, runtime: %llu, vruntime: %llu\n", 
                smp_processor_id(), runtime, vruntime);

        task_struct_display("task: ", tsk);

        pr_debug("\n");
    }

}

// --------------------------------------------------------------


static int kprobe_resched_curr_pre_handler(struct kprobe *p, struct pt_regs *regs)
{

    if  (smp_processor_id() == CPU_ID)
    {
        pr_debug("kprobe_resched_curr_pre_handler:\n");

        current_display();
        
        pr_debug("\n");
    }

    return 0;
}


static int kprobe___schedule_pre_handler(struct kprobe *p, struct pt_regs *regs)
{
    // x86_64 function call convention
    unsigned long args[6] = {
        regs->di,
        regs->si,
        regs->dx,
        regs->cx,
        regs->r8,
        regs->r9,
    };


    bool preempt = args[0];

    if  (smp_processor_id() == CPU_ID)
    {
        pr_debug("kprobe___schedule_pre_handler: preempt: %d\n", preempt);

        current_display();
        
        pr_debug("\n");
    }

    return 0;
}


static int kprobe_generic_fault_handler(struct kprobe *p, struct pt_regs *regs, int trapnr)
{
    pr_info("kprobe_generic_fault_handler: symbol name: %s, p->addr = 0x%p, trap #%dn", 
            p->symbol_name, p->addr, trapnr);
    /* Return 0 because we don't handle the fault. */
    return 0;
}



static int kretprobe_resched_curr_ret_handler(struct kretprobe_instance *ri, struct pt_regs *regs)
{
	unsigned long retval = regs_return_value(regs);
    
    if  (smp_processor_id() == CPU_ID)
    {
        pr_debug("kretprobe_resched_curr_ret_handler:\n");

        current_display();
        
        pr_debug("\n");
    }
    
    
    return 0;
}



// tracepoint_probe_context ----------------------------------------

struct tracepoint_probe_entry {
    char* name;
    struct tracepoint *tp;
    void* probe;
    void* priv;
};

#define TRACEPOINT_PROBE_CONTEXT_MAX_NUM 32

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



// kprobes_init --------------------------------------------------------------------------------

static int kprobes_init(struct kprobe* kps, int num)
{
    int i, j;
    int ret;

    for (i = 0; i < num; i++)
    {
        ret = register_kprobe(&kps[i]);
        if  (ret < 0)
        {   pr_warn("kprobes_init: register_kprobe failed, i: %d, symbol name: %s\n",
                    i, kps[i].symbol_name);
            goto kprobes_init_failed;
        }
    }

    return 0;

kprobes_init_failed:
    for (j = i - 1; j >= 0; j--)
        unregister_kprobe(&kps[j]);

    return ret;
}

static void kprobes_exit(struct kprobe* kps, int num)
{
    int i;

    for (i = num - 1; i >= 0; i--)
        unregister_kprobe(&kps[i]);
}




// kretprobes_init --------------------------------------------------------------------------------

static int kretprobes_init(struct kretprobe* kps, int num)
{
    int i, j;
    int ret;

    for (i = 0; i < num; i++)
    {
        ret = register_kretprobe(&kps[i]);
        if  (ret < 0)
        {   pr_warn("kprobes_init: register_kprobe failed, i: %d, symbol name: %s\n",
                    i, kps[i].kp.symbol_name);
            goto kretprobes_init_failed;
        }
    }

    return 0;

kretprobes_init_failed:
    for (j = i - 1; j >= 0; j--)
        unregister_kretprobe(&kps[j]);

    return ret;
}

static void kretprobes_exit(struct kretprobe* kps, int num)
{
    int i;

    for (i = num - 1; i >= 0; i--)
        unregister_kretprobe(&kps[i]);
}




// module init -----------------------------------------------------

static struct tracepoint_probe_context sched_probes = {
    .entries = {

        {
            .name = "sched_switch",
            .probe = trace_sched_switch_handler,
            .priv = NULL,
        },

        // {
        //     .name = "sched_stat_runtime",
        //     .probe = trace_sched_stat_runtime_handler,
        //     .priv = NULL,
        // },

    },
    .init_num = 1
};



#define kprobe_num 1

static struct kprobe kprobes[kprobe_num] = {
    // {
    //     .symbol_name = "resched_curr",
    //     .pre_handler = kprobe_resched_curr_pre_handler,
    //     .fault_handler = kprobe_generic_fault_handler,
    // },
    {
        .symbol_name = "__schedule",
        .pre_handler = kprobe___schedule_pre_handler,
        .fault_handler = kprobe_generic_fault_handler,
    },
};

#define kretprobe_num 1

static struct kretprobe kretprobes[kretprobe_num] = {
    {
        .kp = {
            .symbol_name = "resched_curr",
        },
	    .handler = kretprobe_resched_curr_ret_handler,
	    .maxactive = 64,
    },

};



static int __init need_resched_display_init(void)
{
    int ret;
    pr_debug("need_resched_display_init begin\n");

    // probe_sched_switch_count = alloc_percpu(u64);

    ret = tracepoint_probe_context_find_tracepoints(&sched_probes);
    if  (ret < 0)
    {   pr_warn("need_resched_display_init: find tracepoints failed\n");
        return ret;
    }
    
    ret = tracepoint_probe_context_register_probes(&sched_probes);
    if  (ret < 0)
    {   pr_warn("need_resched_display_init: register probes failed\n");
        return ret;
    }

    ret = kprobes_init(kprobes, kprobe_num);
    if  (ret < 0)
    {   pr_warn("need_resched_display_init: kprobes_init failed\n");
        goto kprobes_init_failed;
    }

    ret = kretprobes_init(kretprobes, kretprobe_num);
    if  (ret < 0)
    {   pr_warn("need_resched_display_init: kretprobes_init failed\n");
        goto kretprobes_init_failed;
    }


    pr_debug("need_resched_display_init end\n");
    return 0;

kretprobes_init_failed:
    kprobes_exit(kprobes, kprobe_num);
kprobes_init_failed:
    tracepoint_probe_context_unregister_probes(&sched_probes);

    pr_debug("need_resched_display_init failed\n");
    return ret;
}

static void __exit need_resched_display_exit(void)
{
    pr_debug("need_resched_display_exit begin\n");

    kretprobes_exit(kretprobes, kretprobe_num);

    kprobes_exit(kprobes, kprobe_num);

    tracepoint_probe_context_unregister_probes(&sched_probes);

    // free_percpu(probe_sched_switch_count);

    pr_debug("need_resched_display_exit end\n");
    pr_debug("------------------------------------------------------------------\n");
}

module_init(need_resched_display_init)
module_exit(need_resched_display_exit)
MODULE_LICENSE("GPL");
