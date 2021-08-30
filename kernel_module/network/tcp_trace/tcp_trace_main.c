

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>

#include "hook_func_lib.h"
#include <trace/events/sched.h>


#include <linux/tcp.h>
#include <net/tcp.h>
#include <net/sock.h>






static inline unsigned long x86_64_get_regs_arg(struct pt_regs *regs, int index)
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

    if  (!(index >= 0 && index < 6))
        return 0;

    return args[index];
}


// sock common -----------------------------------------------

static bool sock_filter(const struct sock* sk)
{
    u16 local_port, remote_port, family;

    if  (!sk)
        return false;

    local_port = sk->sk_num;
    remote_port = ntohs(sk->sk_dport);

    family = sk->sk_family;


    

    return true;
}


static void sock_common_display(const struct sock* sk)
{
    u16 local_port, remote_port, family;

    if  (!sk) return;

    local_port = sk->sk_num;
    remote_port = ntohs(sk->sk_dport);
    family = sk->sk_family;

    pr_debug("sock_common_display: local_port: %d, remote_port: %d, family: %d\n", 
            local_port, remote_port, family);
}


static void sock_filter_and_display(const struct sock* sk, const char* prefix)
{
    if  (!sock_filter(sk))
        return;

    pr_debug("%s\n", prefix);
    sock_common_display(sk);
}


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




static int kprobe_tcp_v4_syn_recv_sock_pre_handler(struct kprobe *p, struct pt_regs *regs)
{
    // struct sock* listen_sock = (struct sock*)args[0];
    struct sock* req_sock = (struct sock*)x86_64_get_regs_arg(regs, 2);

    // pr_debug("kprobe_tcp_v4_syn_recv_sock_pre_handler:\n");
    // if  (!sock_filter(req_sock))
    //     return 0;

    // sock_common_display(req_sock);
    sock_filter_and_display(req_sock, "kprobe_tcp_v4_syn_recv_sock_pre_handler: ");

    /* A dump_stack() here will give a stack backtrace */
    // dump_stack();

    return 0;
}



static int kretprobe_inet_csk_accept_ret_handler(struct kretprobe_instance *ri, struct pt_regs *regs)
{
    struct sock* newsk = (struct sock*)regs_return_value(regs);

    sock_filter_and_display(newsk, "kretprobe_inet_csk_accept_ret_handler: ");

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
        .symbol_name	= "tcp_v4_syn_recv_sock",
        .pre_handler = kprobe_tcp_v4_syn_recv_sock_pre_handler,
    },
};



#define kretprobe_num 1

static struct kretprobe kretprobes[kretprobe_num] = {
    {
        .kp = {
            .symbol_name = "inet_csk_accept",
        },
	    .handler = kretprobe_inet_csk_accept_ret_handler,
	    .maxactive = 64,
    },
};




static int __init tcp_trace_init(void)
{
    int ret;
    pr_debug("tcp_trace_init begin\n");

    ret = tracepoint_probe_context_find_tracepoints(&sched_probes);
    if  (ret < 0)
    {   pr_warn("find tracepoints failed\n");
        return ret;
    }

    ret = tracepoint_probe_context_register_probes(&sched_probes);
    if  (ret < 0)
    {   pr_warn("register trace probes failed\n");
        return ret;
    }

    ret = kprobes_init(kprobes, kprobe_num);
    if  (ret < 0)
    {   pr_warn("register kprobes failed\n");
        goto kprobes_init_failed;
    }
       
    ret = kretprobes_init(kretprobes, kretprobe_num);
    if  (ret < 0)
    {   pr_warn("register kretprobes failed\n");
        goto kretprobes_init_failed;
    }

    pr_debug("tcp_trace_init end\n");
    return 0;

kretprobes_init_failed:
    kprobes_exit(kprobes, kprobe_num);
kprobes_init_failed:
    tracepoint_probe_context_unregister_probes(&sched_probes);

    return ret;
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
