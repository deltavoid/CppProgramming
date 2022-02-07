

#include "hook_func_lib.h"
#include "tcp_trace.h"





static void tcp_sock_display(struct sock* sk)
{
    struct tcp_sock* tp = (struct tcp_sock*)sk;

    // pr_debug("bytes_acked:     0x%016lx\n", tp->bytes_acked);
    // pr_debug("bytes_received:  0x%016lx\n", tp->bytes_received);
    // lose bits
    pr_debug("bytes_acked:     0x%08x,   bytes_received:  0x%08x\n", (unsigned)tp->bytes_acked, (unsigned)tp->bytes_received);
    pr_debug("snd_una:         0x%08x,   copied_seq:      0x%08x\n", tp->snd_una, tp->copied_seq);
    pr_debug("snd_nxt:         0x%08x,   rcv_nxt:         0x%08x\n", tp->snd_nxt, tp->rcv_nxt);
    pr_debug("write_seq:       0x%08x,   recv not copy:   0x%08x\n", tp->write_seq, tp->rcv_nxt - tp->copied_seq);
    pr_debug("send not ack:    0x%08x,   rcv_wnd:         0x%08x\n", tp->snd_nxt - tp->snd_una, tp->rcv_wnd);
    pr_debug("write not send:  0x%08x\n", tp->write_seq - tp->snd_nxt);
    pr_debug("snd_wnd:         0x%08x\n", tp->snd_wnd);
    pr_debug("srtt_us:           %8d\n", tp->srtt_us >> 3);

}

static void tcp_recv_skb_display(struct sk_buff* skb)
{
    struct tcphdr* th = (struct tcphdr*)skb->data;

    pr_debug("recv ack:        0x%08x,   recv seq:        0x%08x\n", ntohl(th->ack_seq), ntohl(th->seq));
    pr_debug("recv wnd:        0x%08x,   recv len:        0x%08x\n", ntohs(th->window), skb->len - (th->doff << 2));
}

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
    struct sk_buff* skb = (struct sk_buff*)x86_64_get_regs_arg(regs, 1);
    struct kretprobe_tcp_common_ctx* ctx = (struct kretprobe_tcp_common_ctx*)ri->data;
    ctx->sk = NULL;

    if  (!sock_filter_and_display(sk, 2, "kprobe:tcp_rcv_established"))
        return 0;

    ctx->sk = sk;

    tcp_sock_display(sk);

    tcp_recv_skb_display(skb);

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

    tcp_sock_display(sk);


    // pr_debug("\n");
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

    tcp_sock_display(sk);

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

    tcp_sock_display(sk);

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

    tcp_sock_display(sk);

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

    tcp_sock_display(sk);

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



static int kretprobe_entry__tcp_write_xmit(struct kretprobe_instance *ri, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);
    struct kretprobe_tcp_common_ctx* ctx = (struct kretprobe_tcp_common_ctx*)ri->data;
    ctx->sk = NULL;

    if  (!sock_filter_and_display(sk, 2, "kprobe:tcp_write_xmit"))
        return 0;

    ctx->sk = sk;

    tcp_sock_display(sk);

    // pr_debug("\n");
    return 0;
}


static int kretprobe__tcp_write_xmit(struct kretprobe_instance *ri, struct pt_regs *regs)
{
    struct kretprobe_tcp_common_ctx* ctx = (struct kretprobe_tcp_common_ctx*)ri->data;
    struct sock* sk = ctx->sk;

    if  (!sk)
        return 0;

    sock_common_display(sk, "kretprobe:tcp_write_xmit");

    tcp_sock_display(sk);

    // pr_debug("\n");
    return 0;
}

const struct kretprobe kretprobe_hook__tcp_write_xmit = {
    .kp = {
        .symbol_name = "tcp_write_xmit",
    },
    .entry_handler = kretprobe_entry__tcp_write_xmit,
    .handler = kretprobe__tcp_write_xmit,
    .data_size = sizeof(struct kretprobe_tcp_common_ctx),
    .maxactive = 64,
};




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

#define kprobe_num 8

static struct kprobe kprobes[kprobe_num] = {

    
    // {
    //     .symbol_name	= "tcp_rcv_established",
    //     .pre_handler = kprobe__tcp_rcv_established,
    // },
    {
        .symbol_name	= "__tcp_transmit_skb",
        .pre_handler = kprobe____tcp_transmit_skb,
    },
    // {
    //     .symbol_name	= "tcp_write_xmit",
    //     .pre_handler = kprobe__tcp_write_xmit,
    // },


     
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


#define kretprobe_num 4

static struct kretprobe kretprobes[kretprobe_num] = {

    kretprobe_hook__tcp_rcv_established,
    kretprobe_hook__tcp_recvmsg,
    kretprobe_hook__tcp_sendmsg,
    kretprobe_hook__tcp_write_xmit,

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

