

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





static void trace_local_timer_entry_handler(int id)
{
    if  (smp_processor_id() == 0)
    {

        pr_debug("probe_local_timer_entry, jiffies: %ld\n", jiffies);
        preempt_count_display();

        dump_stack();

        printk("\n");
    }

}



static const char symbol[] = "tcp_v4_syn_recv_sock";

static int kprobe_symbol_pre_handler(struct kprobe *p, struct pt_regs *regs)
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

    pr_debug("kprobe_symbol_pre_handler: symbol name: %s, symbol addr: 0x%lx, "
            "arg0: %lu, arg1, %lu, arg2: %lu, arg3: %lu, arg4: %lu, arg5: %lu, "
            "preempt_count: 0x%x\n",
            p->symbol_name, (unsigned long)p->addr,
            args[0], args[1], args[2], args[3], args[4], args[5],
            preempt_count());

    /* A dump_stack() here will give a stack backtrace */
    // dump_stack();

    return 0;
}



static void kprobe_symbol_post_handler(struct kprobe *p, struct pt_regs *regs,
                unsigned long flags)
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

    pr_debug("kprobe_symbol_post_handler: symbol name: %s, symbol addr: 0x%lx, flags: 0x%lx "
            "arg0: %lu, arg1, %lu, arg2: %lu, arg3: %lu, arg4: %lu, arg5: %lu, "
            "preempt_count: 0x%x\n",
            p->symbol_name, (unsigned long)p->addr, flags,
            args[0], args[1], args[2], args[3], args[4], args[5],
            preempt_count());

}



static int kprobe_generic_fault_handler(struct kprobe *p, struct pt_regs *regs, int trapnr)
{
    pr_info("kprobe_generic_fault_handler: symbol name: %s, p->addr = 0x%p, trap #%dn", 
            p->symbol_name, p->addr, trapnr);
    /* Return 0 because we don't handle the fault. */
    return 0;
}



struct my_data {
	ktime_t entry_stamp;
};

static int kretprobe_symbol_entry_handler(struct kretprobe_instance *ri, struct pt_regs *regs)
{
	struct my_data *data;

	// if (!current->mm)
	// 	return 1;	/* Skip kernel threads */

	data = (struct my_data *)ri->data;
	data->entry_stamp = ktime_get();
	return 0;
}



static int kretprobe_symbol_ret_handler(struct kretprobe_instance *ri, struct pt_regs *regs)
{
	unsigned long retval = regs_return_value(regs);
	struct my_data *data = (struct my_data *)ri->data;
	s64 delta;
	ktime_t now;

	now = ktime_get();
	delta = ktime_to_ns(ktime_sub(now, data->entry_stamp));
	pr_info("%s returned %lu and took %lld ns to execute\n",
			symbol, retval, (long long)delta);
	return 0;
}



// module init -----------------------------------------------------

static struct tracepoint_probe_context sched_probes = {
    .entries = {
        {
            .name = "local_timer_entry",
            .probe = trace_local_timer_entry_handler,
            .priv = NULL,
        },
    },
    .init_num = 0
};


#define kprobe_num 1

static struct kprobe kprobes[kprobe_num] = {
    {
        .symbol_name	= symbol,
        .pre_handler = kprobe_symbol_pre_handler,
        // .post_handler = kprobe_symbol_post_handler,
        // .fault_handler = kprobe_generic_fault_handler,
    },
};



#define kretprobe_num 1

static struct kretprobe kretprobes[kretprobe_num] = {
    {
        .kp = {
            .symbol_name = symbol,
        },
	    .handler = kretprobe_symbol_ret_handler,
	    // .entry_handler = kretprobe_symbol_entry_handler,
	    // .data_size = sizeof(struct my_data),
	    .maxactive = 64,
    },
};




static int __init tcp_trace_init(void)
{
    pr_debug("tcp_trace_init begin\n");


    if  (tracepoint_probe_context_find_tracepoints(&sched_probes) < 0)
    {   pr_warn("find tracepoints failed\n");
        return -1;
    }

    if  (tracepoint_probe_context_register_probes(&sched_probes) < 0)
    {   pr_warn("register probes failed\n");
        return -1;
    }

    if  (kprobes_init(kprobes, kprobe_num) < 0)
        return -1;

    
    if  (kretprobes_init(kretprobes, kretprobe_num) < 0)
        return -1;

    pr_debug("tcp_trace_init end\n");
    return 0;
}

static void __exit tcp_trace_exit(void)
{
    pr_debug("tcp_trace_exit begin\n");

    tracepoint_probe_context_unregister_probes(&sched_probes);

    kprobes_exit(kprobes, kprobe_num);

    kretprobes_exit(kretprobes, kretprobe_num);
    

    pr_debug("tcp_trace_exit end\n");
    pr_debug("------------------------------------------------------------------\n");
}

module_init(tcp_trace_init)
module_exit(tcp_trace_exit)
MODULE_LICENSE("GPL");
