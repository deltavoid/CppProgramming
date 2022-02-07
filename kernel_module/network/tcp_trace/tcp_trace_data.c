


#include "hook_func_lib.h"
#include "tcp_trace.h"




static int kprobe__tcp_rcv_established(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);

    if  (!sock_filter_and_display(sk, 3, "kprobe:tcp_rcv_established"))
        return 0;

    // pr_debug("\n");
    return 0;
}







// tcp_recvmsg

static int kretprobe_entry__tcp_recvmsg(struct kretprobe_instance *ri, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);
    struct kretprobe_tcp_common_ctx* ctx = (struct kretprobe_tcp_common_ctx*)ri->data;
    ctx->sk = NULL;

    if  (!sock_filter_and_display(sk, 2, "kprobe:tcp_recvmsg"))
        return 0;

    ctx->sk = sk;

    // pr_debug("\n");
    return 0;
}


static int kretprobe__tcp_recvmsg(struct kretprobe_instance *ri, struct pt_regs *regs)
{
    struct kretprobe_tcp_common_ctx* ctx = (struct kretprobe_tcp_common_ctx*)ri->data;
    struct sock* sk = ctx->sk;

    if  (!sk)
        return 0;

    sock_common_display(sk, "kretprobe:tcp_recvmsg");

    pr_debug("\n");
    return 0;
}

const struct kretprobe kretprobe_hook__tcp_recvmsg = {
    .kp = {
        .symbol_name = "tcp_recvmsg",
    },
    .entry_handler = kretprobe_entry__tcp_recvmsg,
    .handler = kretprobe__tcp_recvmsg,
    .data_size = sizeof(struct kretprobe_tcp_common_ctx),
    .maxactive = 64,
};







// tcp_sendmsg

static int kretprobe_entry__tcp_sendmsg(struct kretprobe_instance *ri, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);
    struct kretprobe_tcp_common_ctx* ctx = (struct kretprobe_tcp_common_ctx*)ri->data;
    ctx->sk = NULL;

    if  (!sock_filter_and_display(sk, 2, "kprobe:tcp_sendmsg"))
        return 0;

    ctx->sk = sk;

    // pr_debug("\n");
    return 0;
}



static int kretprobe__tcp_sendmsg(struct kretprobe_instance *ri, struct pt_regs *regs)
{
    struct kretprobe_tcp_common_ctx* ctx = (struct kretprobe_tcp_common_ctx*)ri->data;
    struct sock* sk = ctx->sk;

    if  (!sk)
        return 0;

    sock_common_display(sk, "kretprobe:tcp_sendmsg");

    pr_debug("\n");
    return 0;
}

const struct kretprobe kretprobe_hook__tcp_sendmsg = {
    .kp = {
        .symbol_name = "tcp_sendmsg",
    },
    .entry_handler = kretprobe_entry__tcp_sendmsg,
    .handler = kretprobe__tcp_sendmsg,
    .data_size = sizeof(struct kretprobe_tcp_common_ctx),
    .maxactive = 64,
};


static int kprobe__tcp_write_xmit(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);

    if  (!sock_filter_and_display(sk, 3, "kprobe:tcp_write_xmit"))
        return 0;

    // pr_debug("\n");
    return 0;
}




static int kprobe____tcp_transmit_skb(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);

    if  (!sock_filter_and_display(sk, 3, "kprobe:__tcp_transmit_skb"))
        return 0;

    // pr_debug("\n");
    return 0;
}





// init -----------------------------------------

#define kprobe_num 3

static struct kprobe kprobes[kprobe_num] = {

    // kprobe_hook__tcp_conn_request,


    
    {
        .symbol_name	= "tcp_rcv_established",
        .pre_handler = kprobe__tcp_rcv_established,
    },
    {
        .symbol_name	= "__tcp_transmit_skb",
        .pre_handler = kprobe____tcp_transmit_skb,
    },
    {
        .symbol_name	= "tcp_write_xmit",
        .pre_handler = kprobe__tcp_write_xmit,
    },




};


#define kretprobe_num 2

static struct kretprobe kretprobes[kretprobe_num] = {

    kretprobe_hook__tcp_recvmsg,
    kretprobe_hook__tcp_sendmsg,

};




int tcp_trace_data_init(void)
{
    int ret;

    pr_debug("tcp_trace_data_init: 1\n");


    ret = kprobes_init(kprobes, kprobe_num);
    if  (ret < 0)
    {   pr_warn("register kprobes_failed\n");
    }


    ret = kretprobes_init(kretprobes, kretprobe_num);
    if  (ret < 0)
    {   pr_warn("register kretprobes failed\n");
    }



    pr_debug("tcp_trace_data_init: 2, end\n");
    return 0;
}

void tcp_trace_data_exit(void)
{
    pr_debug("tcp_trace_data_exit: 1\n");

    kprobes_exit(kprobes, kprobe_num);

    kretprobes_exit(kretprobes, kretprobe_num);


    pr_debug("tcp_trace_data_exit: 2, end\n");
}

