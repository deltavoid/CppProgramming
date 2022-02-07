

#include "hook_func_lib.h"
#include "tcp_trace.h"


// tcp_conn_request

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



// tcp_close

static int kretprobe_entry__tcp_close(struct kretprobe_instance *ri, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);
    struct kretprobe_tcp_common_ctx* ctx = (struct kretprobe_tcp_common_ctx*)ri->data;
    ctx->sk = NULL;

    if  (!sock_filter_and_display(sk, 2, "kprobe:tcp_close"))
        return 0;

    ctx->sk = sk;

    // pr_debug("\n");
    return 0;
}


static int kretprobe__tcp_close(struct kretprobe_instance *ri, struct pt_regs *regs)
{
    struct kretprobe_tcp_common_ctx* ctx = (struct kretprobe_tcp_common_ctx*)ri->data;
    struct sock* sk = ctx->sk;

    if  (!sk)
        return 0;

    // sock_common_display(sk, "kretprobe:tcp_close");

    // for close may destroy the sock, so just print sk addr.
    pr_debug("%-32s  sock: 0x%lx\n", "kretprobe:tcp_close", (unsigned long)sk);

    pr_debug("\n");
    return 0;
}

const struct kretprobe kretprobe_hook__tcp_close = {
    .kp = {
        .symbol_name = "tcp_close",
    },
    .entry_handler = kretprobe_entry__tcp_close,
    .handler = kretprobe__tcp_close,
    .data_size = sizeof(struct kretprobe_tcp_common_ctx),
    .maxactive = 64,
};



// tcp_v4_send_synack

static int kprobe__tcp_v4_send_synack(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 3);

    if  (!sock_filter_and_display(sk, 2, "kprobe:tcp_v4_send_synack"))
        return 0;

    // pr_debug("\n");
    return 0;
}

const struct kprobe kprobe_hook__tcp_v4_send_synack = 
{
    .symbol_name	= "tcp_v4_send_synack",
    .pre_handler = kprobe__tcp_v4_send_synack,
};


// tcp_rcv_state_process

static int kretprobe_entry__tcp_rcv_state_process(struct kretprobe_instance *ri, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);
    struct kretprobe_tcp_common_ctx* ctx = (struct kretprobe_tcp_common_ctx*)ri->data;

    ctx->sk = NULL;

    if  (!sock_filter_and_display(sk, 2, "kprobe:tcp_rcv_state_process"))
        return 0;

    // *shifted_tid_p = get_shifted_tid();
    // ctx->shifted_tid = get_shifted_tid();
    ctx->sk = sk;


    // pr_debug("\n");
    return 0;

}

static int kretprobe__tcp_rcv_state_process(struct kretprobe_instance *ri, struct pt_regs *regs)
{
    struct kretprobe_tcp_common_ctx* ctx = (struct kretprobe_tcp_common_ctx*)ri->data;
    // unsigned shifted_tid = ctx->shifted_tid;
    struct sock* sk = ctx->sk;

    if  (!sk)
        return 0;

    // struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);
    

    // if  (!sock_filter_and_display(sk, 2, "kretprobe:tcp_rcv_state_process"))
    //     return 0;

    sock_common_display(sk, "kretprobe:tcp_rcv_state_process");

    // pr_debug("\n");
    return 0;
}

const struct kretprobe kretprobe_hook__tcp_rcv_state_process = {
    .kp = {
        .symbol_name = "tcp_rcv_state_process",
    },
    .entry_handler = kretprobe_entry__tcp_rcv_state_process,
    .handler = kretprobe__tcp_rcv_state_process,
    .data_size = sizeof(struct kretprobe_tcp_common_ctx),
    .maxactive = 64,
};


// tcp_v4_do_rcv

static int kretprobe_entry__tcp_v4_do_rcv(struct kretprobe_instance *ri, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);
    struct kretprobe_tcp_common_ctx* ctx = (struct kretprobe_tcp_common_ctx*)ri->data;
    ctx->sk = NULL;

    if  (!sock_filter_and_display(sk, 2, "kprobe:tcp_v4_do_rcv"))
        return 0;

    ctx->sk = sk;

    // pr_debug("\n");
    return 0;
}

static int kretprobe__tcp_v4_do_rcv(struct kretprobe_instance *ri, struct pt_regs *regs)
{
    struct kretprobe_tcp_common_ctx* ctx = (struct kretprobe_tcp_common_ctx*)ri->data;
    struct sock* sk = ctx->sk;

    if  (!sk)
        return 0;

    sock_common_display(sk, "kretprobe:tcp_v4_do_rcv");

    pr_debug("\n");
    return 0;
}

const struct kretprobe kretprobe_hook__tcp_v4_do_rcv = {
    .kp = {
        .symbol_name = "tcp_v4_do_rcv",
    },
    .entry_handler = kretprobe_entry__tcp_v4_do_rcv,
    .handler = kretprobe__tcp_v4_do_rcv,
    .data_size = sizeof(struct kretprobe_tcp_common_ctx),
    .maxactive = 64,
};



// init -----------------------------------------

#define kprobe_num 2

static struct kprobe kprobes[kprobe_num] = {

    kprobe_hook__tcp_conn_request,
    kprobe_hook__tcp_v4_send_synack,


};


#define kretprobe_num 3

static struct kretprobe kretprobes[kretprobe_num] = {

    kretprobe_hook__tcp_close,
    kretprobe_hook__tcp_rcv_state_process,
    kretprobe_hook__tcp_v4_do_rcv,

};




int tcp_trace_conn_init(void)
{
    int ret;

    pr_debug("tcp_trace_conn_init: 1\n");


    ret = kprobes_init(kprobes, kprobe_num);
    if  (ret < 0)
    {   pr_warn("register kprobes_failed\n");
    }


    ret = kretprobes_init(kretprobes, kretprobe_num);
    if  (ret < 0)
    {   pr_warn("register kretprobes failed\n");
    }



    pr_debug("tcp_trace_conn_init: 2, end\n");
    return 0;
}

void tcp_trace_conn_exit(void)
{
    pr_debug("tcp_trace_conn_exit: 1\n");

    kprobes_exit(kprobes, kprobe_num);

    kretprobes_exit(kretprobes, kretprobe_num);


    pr_debug("tcp_trace_conn_exit: 2, end\n");
}
