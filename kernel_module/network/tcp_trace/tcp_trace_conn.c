

#include "hook_func_lib.h"
#include "tcp_trace.h"


// estab ----------------------------------------

// tcp_v4_do_rcv

static int kprobe__tcp_v4_do_rcv(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);

    if  (!sock_filter_and_display(sk, 2, "kprobe:tcp_v4_do_rcv"))
        return 0;

    // pr_debug("\n");
    return 0;
}

const struct kprobe kprobe_hook__tcp_v4_do_rcv = {
    .symbol_name	= "tcp_v4_do_rcv",
    .pre_handler = kprobe__tcp_v4_do_rcv,
};

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




// tcp_rcv_state_process

static int kprobe__tcp_rcv_state_process(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);
    

    if  (!sock_filter_and_display(sk, 2, "kprobe:tcp_rcv_state_process"))
        return 0;


    // pr_debug("\n");
    return 0;
}

const struct kprobe kprobe_hook__tcp_rcv_state_process = {
    .symbol_name	= "tcp_rcv_state_process",
    .pre_handler = kprobe__tcp_rcv_state_process,
};



static int kretprobe_entry__tcp_rcv_state_process(struct kretprobe_instance *ri, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);
    struct sk_buff* skb = (struct sk_buff*)x86_64_get_regs_arg(regs, 1);
    struct kretprobe_tcp_common_ctx* ctx = (struct kretprobe_tcp_common_ctx*)ri->data;

    ctx->sk = NULL;

    if  (!sock_filter_and_display(sk, 2, "kprobe:tcp_rcv_state_process"))
        return 0;

    // *shifted_tid_p = get_shifted_tid();
    // ctx->shifted_tid = get_shifted_tid();
    ctx->sk = sk;


    tcp_recv_skb_display(skb);

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



// ---

static int kprobe__tcp_check_req(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 2);

    if  (!sock_filter_and_display(sk, 2, "kprobe:tcp_check_req"))
        return 0;

    // pr_debug("\n");
    return 0;
}

const struct kprobe kprobe_hook__tcp_check_req = {
    .symbol_name	= "tcp_check_req",
    .pre_handler = kprobe__tcp_check_req,
};

// tcp_v4_syn_recv_sock

static int kprobe__tcp_v4_syn_recv_sock(struct kprobe *p, struct pt_regs *regs)
{
    struct sk_buff* skb = (struct sk_buff*)x86_64_get_regs_arg(regs, 1);
    struct sock* req_sock = (struct sock*)x86_64_get_regs_arg(regs, 2);

    if  (!sock_filter_and_display(req_sock, 2, "kprobe:tcp_v4_syn_recv_sock"))
        return 0;

    // dump_stack();

    tcp_recv_skb_display(skb);

    // pr_debug("\n");
    return 0;
}

const struct kprobe kprobe_hook__tcp_v4_syn_recv_sock = {
    .symbol_name	= "tcp_v4_syn_recv_sock",
    .pre_handler = kprobe__tcp_v4_syn_recv_sock,
};

static int kretprobe__tcp_v4_syn_recv_sock(struct kretprobe_instance *ri, struct pt_regs *regs)
{
    struct sock* req_sock = (struct sock*)regs_return_value(regs);

    if  (!sock_filter_and_display(req_sock, 2, "kretprobe:tcp_v4_syn_recv_sock"))
        return 0;

    // pr_debug("\n");
    return 0;
}

const struct kretprobe kretprobe_hook__tcp_v4_syn_recv_sock = {
        .kp = {
            .symbol_name = "tcp_v4_syn_recv_sock",
        },
	    .handler = kretprobe__tcp_v4_syn_recv_sock,
	    .maxactive = 64,
};

// ---

static int kprobe__tcp_create_openreq_child(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 1);

    if  (!sock_filter_and_display(sk, 2, "kprobe:tcp_create_openreq_child"))
        return 0;

    // pr_debug("\n");
    return 0;
}

const struct kprobe kprobe_hook__tcp_create_openreq_child = {
    .symbol_name	= "tcp_create_openreq_child",
    .pre_handler = kprobe__tcp_create_openreq_child,
};


// ---

static int kprobe__inet_csk_clone_lock(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 1);

    if  (!sock_filter_and_display(sk, 2, "kprobe:inet_csk_clone_lock"))
        return 0;

    // pr_debug("\n");
    return 0;
}

const struct kprobe kprobe_hook__inet_csk_clone_lock = {
    .symbol_name	= "inet_csk_clone_lock",
    .pre_handler = kprobe__inet_csk_clone_lock,
};

// ---

static int kprobe__tcp_child_process(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 1);

    if  (!sock_filter_and_display(sk, 2, "kprobe:tcp_child_process"))
        return 0;

    // pr_debug("\n");
    return 0;
}

const struct kprobe kprobe_hook__tcp_child_process = {
    .symbol_name	= "tcp_child_process",
    .pre_handler = kprobe__tcp_child_process,
};

// inet_csk_accept

static int kretprobe_inet_csk_accept(struct kretprobe_instance *ri, struct pt_regs *regs)
{
    struct sock* newsk = (struct sock*)regs_return_value(regs);

    if  (!sock_filter_and_display(newsk, 2, "kretprobe:inet_csk_accept"))
        return 0;

    pr_debug("\n");
    return 0;
}

const struct kretprobe kretprobe_hook__inet_csk_accept = {
        .kp = {
            .symbol_name = "inet_csk_accept",
        },
	    .handler = kretprobe_inet_csk_accept,
	    .maxactive = 64,
};


static int kprobe__tcp_set_state(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);
    int dest_state = (int)x86_64_get_regs_arg(regs, 1);

    // if  (!sock_filter_and_display(sk, "kprobe__tcp_set_state"))
    //     return 0;
    if  (!sock_filter(sk, 2))
        return 0;

    // pr_debug("kprobe:tcp_set_state: %s -> %s\n", 
    //         tcp_state_desc[sk->sk_state], tcp_state_desc[dest_state]);

    // current_display();
    sock_common_display(sk, "kprobe:tcp_set_state");
    
    pr_debug("%s -> %s\n", 
            tcp_state_desc[sk->sk_state], tcp_state_desc[dest_state]);


    // dump_stack();

    // pr_debug("\n");
    return 0;
}

const struct kprobe kprobe_hook__tcp_set_state = {
    .symbol_name	= "tcp_set_state",
    .pre_handler = kprobe__tcp_set_state,
};



// connect ----------------------------------


// // not dest port here
// static int kprobe__tcp_v4_connect(struct kprobe *p, struct pt_regs *regs)
// {
//     struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);
    
//     if  (!sock_filter_and_display(sk, "kprobe:tcp_v4_connect"))
//         return 0;

//     // pr_debug("\n");
//     return 0;
// }


static int kprobe__inet_hash_connect(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 1);
    
    if  (!sock_filter_and_display(sk, 2, "kprobe:inet_hash_connect"))
        return 0;

    // pr_debug("\n");
    return 0;
}

static int kprobe__tcp_connect(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);
    
    if  (!sock_filter_and_display(sk, 2, "kprobe:tcp_connect"))
        return 0;

    // pr_debug("\n");
    return 0;
}


static int kprobe__tcp_finish_connect(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);
    
    if  (!sock_filter_and_display(sk, 2, "kprobe:tcp_finish_connect"))
        return 0;

    // pr_debug("\n");
    return 0;
}




// close -------------------------------------

// tcp_close


static int kprobe__tcp_close(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);

    if  (!sock_filter_and_display(sk, 2, "kprobe:tcp_close"))
        return 0;

    // pr_debug("\n");
    return 0;
}


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




static int kprobe__tcp_fin(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);

    if  (!sock_filter_and_display(sk, 2, "kprobe:tcp_fin"))
        return 0;

    // pr_debug("\n");
    return 0;
}


static int kprobe__tcp_time_wait(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);

    if  (!sock_filter_and_display(sk, 2, "kprobe:tcp_time_wait"))
        return 0;

    // pr_debug("\n");
    return 0;
}

static int kprobe__inet_twsk_kill(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);

    if  (!sock_filter_and_display(sk, 2, "kprobe:inet_twsk_kill"))
        return 0;

    // dump_stack();
    // pr_debug("\n");
    return 0;
}


static int kprobe__tcp_timewait_state_process(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);

    if  (!sock_filter_and_display(sk, 2, "kprobe:tcp_timewait_state_process"))
        return 0;

    // pr_debug("\n");
    return 0;
}



static int kprobe__tcp_done(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);

    if  (!sock_filter_and_display(sk, 2, "kprobe:tcp_done"))
        return 0;

    // pr_debug("\n");
    return 0;
}


static int kprobe__inet_csk_destroy_sock(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);

    if  (!sock_filter_and_display(sk, 2, "kprobe:inet_csk_destroy_sock"))
        return 0;

    // pr_debug("\n");
    return 0;
}



// reset ----------------------------------------------------------

static void trace__tcp_receive_reset(struct sock* sk)
{
    if  (!sock_filter_and_display(sk, 2, "trace:tcp_receive_reset"))
        return;

    // pr_debug("\n");
}




static void trace__tcp_send_reset(const struct sock *sk, const struct sk_buff *skb)
{
    if  (!sock_filter_and_display(sk, 2, "trace:tcp_send_reset"))
        return;

    // pr_debug("\n");
}



static int kprobe__tcp_reset(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);

    if  (!sock_filter_and_display(sk, 2, "kprobe:tcp_reset"))
        return 0;

    // pr_debug("\n");
    return 0;
}

static int kprobe__tcp_v4_send_reset(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);

    if  (!sock_filter_and_display(sk, 2, "kprobe:tcp_v4_send_reset"))
        return 0;

    // pr_debug("\n");
    return 0;
}



// init -----------------------------------------

static struct tracepoint_probe_context sched_probes = {
    .entries = {
        {
            .name = "tcp_receive_reset",
            .probe = trace__tcp_receive_reset,
            .priv = NULL,
        },
        {
            .name = "tcp_send_reset",
            .probe = trace__tcp_send_reset,
            .priv = NULL,
        },
    },
    .init_num = 2
};

#define kprobe_num 19

static struct kprobe kprobes[kprobe_num] = {

    /* estab */

    kprobe_hook__tcp_conn_request,
    kprobe_hook__tcp_v4_send_synack,
    kprobe_hook__tcp_check_req,
    kprobe_hook__tcp_v4_syn_recv_sock,
    kprobe_hook__tcp_create_openreq_child,
    kprobe_hook__inet_csk_clone_lock,
    kprobe_hook__tcp_child_process,

    kprobe_hook__tcp_set_state,

    
    {
        .symbol_name	= "inet_hash_connect",
        .pre_handler = kprobe__inet_hash_connect,
    },    
    {
        .symbol_name	= "tcp_connect",
        .pre_handler = kprobe__tcp_connect,
    },
    {
        .symbol_name	= "tcp_finish_connect",
        .pre_handler = kprobe__tcp_finish_connect,
    },

    /* close */

    {
        .symbol_name	= "tcp_fin",
        .pre_handler = kprobe__tcp_fin,
    },

    {
        .symbol_name	= "tcp_done",
        .pre_handler = kprobe__tcp_done,
    },


    {
        .symbol_name	= "tcp_timewait_state_process",
        .pre_handler = kprobe__tcp_timewait_state_process,
    },
    {
        .symbol_name	= "tcp_time_wait",
        .pre_handler = kprobe__tcp_time_wait,
    },
    {
        .symbol_name	= "inet_twsk_kill",
        .pre_handler = kprobe__inet_twsk_kill,
    },

    {
        .symbol_name	= "inet_csk_destroy_sock",
        .pre_handler = kprobe__inet_csk_destroy_sock,
    },


    /* reset */

    {
        .symbol_name	= "tcp_reset",
        .pre_handler = kprobe__tcp_reset,
    },
    {
        .symbol_name	= "tcp_v4_send_reset",
        .pre_handler = kprobe__tcp_v4_send_reset,
    },    


};


#define kretprobe_num 5

static struct kretprobe kretprobes[kretprobe_num] = {

    kretprobe_hook__tcp_v4_do_rcv,
    kretprobe_hook__tcp_rcv_state_process,
    kretprobe_hook__tcp_v4_syn_recv_sock,
    kretprobe_hook__inet_csk_accept,


    kretprobe_hook__tcp_close,

};




int tcp_trace_conn_init(void)
{
    int ret;
    
    pr_debug("tcp_trace_conn_init: 1\n");

    ret = tracepoint_probe_context_find_tracepoints(&sched_probes);
    if  (ret < 0)
    {   pr_warn("find tracepoints failed\n");
        return ret;
    }

    ret = tracepoint_probe_context_register_probes(&sched_probes);
    if  (ret < 0)
    {   pr_warn("register trace probes failed\n");
        goto tracepoint_failed;
    }

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

    pr_debug("tcp_trace_conn_init: 2, end\n");
    return 0;

kretprobe_failed:
    kprobes_exit(kprobes, kprobe_num);
kprobe_failed:
    tracepoint_probe_context_unregister_probes(&sched_probes);
tracepoint_failed:

    return ret;
}

void tcp_trace_conn_exit(void)
{
    pr_debug("tcp_trace_conn_exit: 1\n");

    tracepoint_probe_context_unregister_probes(&sched_probes);

    kprobes_exit(kprobes, kprobe_num);

    kretprobes_exit(kretprobes, kretprobe_num);


    pr_debug("tcp_trace_conn_exit: 2, end\n");
}
