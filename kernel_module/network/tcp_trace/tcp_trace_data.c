

#include "hook_func_lib.h"
#include "tcp_trace.h"




// send and recv 

// recv -----------------------------

static int kprobe__tcp_rcv_established(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);

    if  (!sock_filter_and_display(sk, 3, "kprobe:tcp_rcv_established"))
        return 0;

    // pr_debug("\n");
    return 0;
}





static int kretprobe_entry__tcp_rcv_established(struct kretprobe_instance *ri, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);
    struct kretprobe_tcp_common_ctx* ctx = (struct kretprobe_tcp_common_ctx*)ri->data;
    ctx->sk = NULL;

    if  (!sock_filter_and_display(sk, 2, "kprobe:tcp_rcv_established"))
        return 0;

    ctx->sk = sk;

    // pr_debug("\n");
    return 0;
}


static int kretprobe__tcp_rcv_established(struct kretprobe_instance *ri, struct pt_regs *regs)
{
    struct kretprobe_tcp_common_ctx* ctx = (struct kretprobe_tcp_common_ctx*)ri->data;
    struct sock* sk = ctx->sk;

    if  (!sk)
        return 0;

    sock_common_display(sk, "kretprobe:tcp_rcv_established");

    pr_debug("\n");
    return 0;
}

const struct kretprobe kretprobe_hook__tcp_rcv_established = {
    .kp = {
        .symbol_name = "tcp_rcv_established",
    },
    .entry_handler = kretprobe_entry__tcp_rcv_established,
    .handler = kretprobe__tcp_rcv_established,
    .data_size = sizeof(struct kretprobe_tcp_common_ctx),
    .maxactive = 64,
};





// tcp_recvmsg


static int kprobe__tcp_recvmsg(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);

    if  (!sock_filter_and_display(sk, 3, "kprobe:tcp_recvmsg"))
        return 0;

    // pr_debug("\n");
    return 0;
}




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





// send ------------------------------

// tcp_sendmsg


static int kprobe__tcp_sendmsg(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);

    if  (!sock_filter_and_display(sk, 3, "kprobe:tcp_sendmsg"))
        return 0;

    // pr_debug("\n");
    return 0;
}

const struct kprobe kprobe_hook__tcp_sendmsg = 
    {
        .symbol_name	= "tcp_sendmsg",
        .pre_handler = kprobe__tcp_sendmsg,
    };


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


// tcp_poll / sock event ---------------------------



static int kprobe__sock_def_wakeup(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);

    if  (!sock_filter_and_display(sk, 3, "kprobe:sock_def_wakeup"))
        return 0;

    // pr_debug("\n");
    return 0;
}

static int kprobe__sock_def_readable(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);

    if  (!sock_filter_and_display(sk, 3, "kprobe:sock_def_readable"))
        return 0;

    // pr_debug("\n");
    return 0;
}

static int kprobe__sock_def_write_space(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);

    if  (!sock_filter_and_display(sk, 3, "kprobe:sock_def_write_space"))
        return 0;

    // pr_debug("\n");
    return 0;
}

static int kprobe__sk_stream_write_space(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);

    if  (!sock_filter_and_display(sk, 3, "kprobe:sk_stream_write_space"))
        return 0;

    // pr_debug("\n");
    return 0;
}




// timer and retrans ----------------------------------------------


static int kprobe__tcp_write_timer(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);

    if  (!sock_filter_and_display(sk, 3, "kprobe:tcp_write_timer"))
        return 0;

    // pr_debug("\n");
    return 0;
}

static int kprobe__tcp_delack_timer(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);

    if  (!sock_filter_and_display(sk, 3, "kprobe:tcp_delack_timer"))
        return 0;

    // pr_debug("\n");
    return 0;
}

static int kprobe__tcp_keepalive_timer(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);

    if  (!sock_filter_and_display(sk, 3, "kprobe:tcp_keepalive_timer"))
        return 0;

    // pr_debug("\n");
    return 0;
}

// cc 




// init -----------------------------------------

#define kprobe_num 9

static struct kprobe kprobes[kprobe_num] = {

    
    // {
    //     .symbol_name	= "tcp_rcv_established",
    //     .pre_handler = kprobe__tcp_rcv_established,
    // },
    {
        .symbol_name	= "__tcp_transmit_skb",
        .pre_handler = kprobe____tcp_transmit_skb,
    },
    {
        .symbol_name	= "tcp_write_xmit",
        .pre_handler = kprobe__tcp_write_xmit,
    },


     
    {
        .symbol_name	= "sock_def_wakeup",
        .pre_handler = kprobe__sock_def_wakeup,
    },
    {
        .symbol_name	= "sock_def_readable",
        .pre_handler = kprobe__sock_def_readable,
    },
    {
        .symbol_name	= "sock_def_write_space",
        .pre_handler = kprobe__sock_def_write_space,
    },
    {
        .symbol_name	= "sk_stream_write_space",
        .pre_handler = kprobe__sk_stream_write_space,
    },


    {
        .symbol_name	= "tcp_write_timer",
        .pre_handler = kprobe__tcp_write_timer,
    },
    {
        .symbol_name	= "tcp_delack_timer",
        .pre_handler = kprobe__tcp_delack_timer,
    },
    {
        .symbol_name	= "tcp_keepalive_timer",
        .pre_handler = kprobe__tcp_keepalive_timer,
    },



};


#define kretprobe_num 3

static struct kretprobe kretprobes[kretprobe_num] = {

    kretprobe_hook__tcp_rcv_established,
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
        goto kprobe_failed;
    }

    ret = kretprobes_init(kretprobes, kretprobe_num);
    if  (ret < 0)
    {   pr_warn("register kretprobes failed\n");
        goto kretprobe_failed;
    }

    pr_debug("tcp_trace_data_init: 2, end\n");
    return 0;

kretprobe_failed:
    kprobes_exit(kprobes, kprobe_num);
kprobe_failed:

    return ret;
}

void tcp_trace_data_exit(void)
{
    pr_debug("tcp_trace_data_exit: 1\n");

    kprobes_exit(kprobes, kprobe_num);

    kretprobes_exit(kretprobes, kretprobe_num);


    pr_debug("tcp_trace_data_exit: 2, end\n");
}

