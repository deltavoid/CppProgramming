

#include "hook_func_lib.h"
#include "tcp_trace.h"



static int kprobe__tcp_conn_request(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 2);

    if  (!sock_filter_and_display(sk, 2, "kprobe:tcp_conn_request"))
        return 0;

    // dump_stack();
    // pr_debug("\n");
    return 0;
}

const struct kprobe kprobe_hook__tcp_conn_request = {
        .symbol_name	= "tcp_conn_request",
        .pre_handler = kprobe__tcp_conn_request,
};



#define kprobe_num 1

static struct kprobe kprobes[kprobe_num] = {

    kprobe_hook__tcp_conn_request,


};


int tcp_trace_conn_init(void)
{
    int ret;

    pr_debug("tcp_trace_conn_init: 1\n");


    ret = kprobes_init(kprobes, kprobe_num);
    if  (ret < 0)
    {   pr_warn("register kprobes_failed\n");
    }

    pr_debug("tcp_trace_conn_init: 2, end\n");
    return 0;
}

void tcp_trace_conn_exit(void)
{
    pr_debug("tcp_trace_conn_exit: 1\n");

    kprobes_exit(kprobes, kprobe_num);


    pr_debug("tcp_trace_conn_exit: 2, end\n");
}
