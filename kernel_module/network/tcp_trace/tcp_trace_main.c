

// #define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>

#include "hook_func_lib.h"
#include <trace/events/sched.h>


#include <linux/tcp.h>
#include <net/tcp.h>
#include <net/sock.h>

#include "tcp_trace.h"




// static inline unsigned long x86_64_get_regs_arg(struct pt_regs *regs, int index)
// {
//     // x86_64 function call convention
//     unsigned long args[6] = {
//         regs->di,
//         regs->si,
//         regs->dx,
//         regs->cx,
//         regs->r8,
//         regs->r9,
//     };

//     if  (!(index >= 0 && index < 6))
//         return 0;

//     return args[index];
// }

// static inline unsigned get_shifted_tid(void)
// {
//     return current->pid + smp_processor_id();
// }


// sock common -----------------------------------------------

struct sock_filter_config {
    u16 local_port, remote_port; // 0 indicates not filter;
    bool enable_ipv4;
    bool enable_ipv6;
    u32 local_addr_ipv4, remote_addr_ipv4; // 0 indicates not filter;
};

static void sock_filter_config_display(struct sock_filter_config* config)
{
    pr_debug("local_port: %d, remote_port: %d\n",
        config->local_port, config->remote_port);

    pr_debug("enable_ipv4: %d, local_addr: %pI4, remote_addr: %pI4\n",
            config->enable_ipv4, &config->local_addr_ipv4, &config->remote_addr_ipv4);
    
    pr_debug("enable ipv6: %d\n", config->enable_ipv6);

}


struct sock_filter_config sock_config = {
    .local_port = 0,
    .remote_port = 0,
    .enable_ipv4 = true,
    .local_addr_ipv4 = 0,
    .remote_addr_ipv4 = 0,
    .enable_ipv6 = true,
};


static int param_local_port = 0, param_remote_port = 0;
module_param_named(local_port, param_local_port, int, 0444);
module_param_named(remote_port, param_remote_port, int, 0444);

static int param_enable_ipv4 = 1, param_enable_ipv6 = 1;
module_param_named(enable_ipv4, param_enable_ipv4, int, 0444);
module_param_named(enable_ipv6, param_enable_ipv6, int, 0444);

static int init_sock_config_from_param(void)
{
    sock_config.local_port = param_local_port;
    sock_config.remote_port = param_remote_port;

    sock_config.enable_ipv4 = param_enable_ipv4;
    sock_config.enable_ipv6 = param_enable_ipv6;

    return 0;
}

static int trace_level = 3;


static bool sock_filter(const struct sock* sk, int func_level)
{
    u16 local_port, remote_port, family;

    if  (!sk)
        return false;

    if  (trace_level < func_level)
        return false;

    local_port = sk->sk_num;
    remote_port = ntohs(sk->sk_dport);

    if  (!(sock_config.local_port == 0
            || local_port == sock_config.local_port))
        return false;

    if  (!(sock_config.remote_port == 0
            || remote_port == sock_config.remote_port))
        return false;


    family = sk->sk_family;
    if  (family == AF_INET)
    {
        u32 local_addr, remote_addr;
        if  (!sock_config.enable_ipv4)
            return false;

        local_addr = sk->sk_rcv_saddr;
        if  (!(sock_config.local_addr_ipv4 == 0
                || local_addr == sock_config.local_addr_ipv4))
            return false;

        remote_addr = sk->sk_daddr;
        if  (!(sock_config.remote_addr_ipv4 == 0
                || remote_addr == sock_config.remote_addr_ipv4))
            return false;
    }
    else if  (family == AF_INET6)
    {
        if  (!sock_config.enable_ipv6)
            return false;
    }


    return true;
}


static const char* tcp_state_desc[TCP_MAX_STATES] = {
    "NONE",
    "TCP_ESTABLISHED",
    "TCP_SYN_SENT",
    "TCP_SYN_RECV",
    "TCP_FIN_WAIT1",
    "TCP_FIN_WAIT2",
    "TCP_TIME_WAIT",
    "TCP_CLOSE",
    "TCP_CLOSE_WAIT",
    "TCP_LAST_ACK",
    "TCP_LISTEN",
    "TCP_CLOSING",
    "TCP_NEW_SYN_RECV",
};

static void sock_common_display(const struct sock* sk, const char* prefix)
{
    u16 local_port, remote_port, family;
    u8 state;

    if  (!sk) return;

    local_port = sk->sk_num;
    remote_port = ntohs(sk->sk_dport);
    family = sk->sk_family;
    state = sk->sk_state;

    // sock_common_display: 
    // pr_debug("local_port: %d, remote_port: %d, state: %s\n", 
    //         local_port, remote_port, tcp_state_desc[state]);

    if  (family == AF_INET)
    {
        u32 local_addr = sk->sk_rcv_saddr;
        u32 remote_addr = sk->sk_daddr;

        pr_debug("%-32s  sock: 0x%lx, local: %pI4:%d, remote: %pI4:%d, state: %s\n",
                prefix, (unsigned long)sk, &local_addr, local_port, &remote_addr, remote_port, tcp_state_desc[state]);
    }
    else if  (family == AF_INET6)
    {
        struct in6_addr local_addr = sk->sk_v6_rcv_saddr;
        struct in6_addr remote_addr = sk->sk_v6_daddr;

        pr_debug("%-32s  sock: 0x%lx, local: [%pI6]:%d, remote: [%pI6]:%d state: %s\n",
                prefix, (unsigned long)sk, &local_addr, local_port, &remote_addr, remote_port, tcp_state_desc[state]);
    }
}

static void current_display(void)
{
    struct task_struct* task_p = current;

    preempt_disable();
    // comm should use get_task_comm
    pr_debug("tid: %d, pid: %d, comm: %s, cpu_id: %d, "
            "preempt_count: 0x%08x, need_resched: %d\n", 
            task_p->pid, task_p->tgid, task_p->comm, smp_processor_id(),
            preempt_count(), test_preempt_need_resched());
    preempt_enable();

    // pr_debug("preempt_count: 0x%08x, test_preempt_need_resched: %d\n", 
    //         preempt_count(), test_preempt_need_resched());
    // pr_debug("test_preempt_need_resched: %d\n", test_preempt_need_resched());
}

bool sock_filter_and_display(const struct sock* sk, int func_level, const char* prefix)
{
    if  (!sock_filter(sk, func_level))
        return false;

    // pr_debug("%s\n", prefix);
    
    // current_display();
    sock_common_display(sk, prefix);
    
    
    return true;
}

// tcp connection hook point ----------------------------------------


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


// // typedef struct sock* sock_pointer;
// struct kretprobe_tcp_common_ctx {
//     struct sock* sk;
// };

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



struct tcp_rcv_state_process__ctx {
    // unsigned shifted_tid;
    struct sock* sk;
};

static int kretprobe_entry__tcp_rcv_state_process(struct kretprobe_instance *ri, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);
    struct tcp_rcv_state_process__ctx* ctx = (struct tcp_rcv_state_process__ctx*)ri->data;

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
    struct tcp_rcv_state_process__ctx* ctx = (struct tcp_rcv_state_process__ctx*)ri->data;
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
    .data_size = sizeof(struct tcp_rcv_state_process__ctx),
    .maxactive = 64,
};


// static int kprobe__tcp_conn_request(struct kprobe *p, struct pt_regs *regs)
// {
//     struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 2);

//     if  (!sock_filter_and_display(sk, 2, "kprobe:tcp_conn_request"))
//         return 0;

//     // dump_stack();
//     // pr_debug("\n");
//     return 0;
// }

// const struct kprobe kprobe_hook__tcp_conn_request = {
//         .symbol_name	= "tcp_conn_request",
//         .pre_handler = kprobe__tcp_conn_request,
// };

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

static int kprobe__tcp_check_req(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 2);

    if  (!sock_filter_and_display(sk, 2, "kprobe:tcp_check_req"))
        return 0;

    // pr_debug("\n");
    return 0;
}

const struct kprobe kprobe_hook__tcp_check_req = 
    {
        .symbol_name	= "tcp_check_req",
        .pre_handler = kprobe__tcp_check_req,
    };

static int kprobe__tcp_v4_syn_recv_sock(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* req_sock = (struct sock*)x86_64_get_regs_arg(regs, 2);

    if  (!sock_filter_and_display(req_sock, 2, "kprobe:tcp_v4_syn_recv_sock"))
        return 0;

    // dump_stack();

    // pr_debug("\n");
    return 0;
}

const struct kprobe kprobe_hook__tcp_v4_syn_recv_sock = 
    {
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

static int kprobe__tcp_create_openreq_child(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 1);

    if  (!sock_filter_and_display(sk, 2, "kprobe:tcp_create_openreq_child"))
        return 0;

    // pr_debug("\n");
    return 0;
}

const struct kprobe kprobe_hook__tcp_create_openreq_child = 
    {
        .symbol_name	= "tcp_create_openreq_child",
        .pre_handler = kprobe__tcp_create_openreq_child,
    };

static int kprobe__inet_csk_clone_lock(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 1);

    if  (!sock_filter_and_display(sk, 2, "kprobe:inet_csk_clone_lock"))
        return 0;

    // pr_debug("\n");
    return 0;
}

const struct kprobe kprobe_hook__inet_csk_clone_lock = 
    {
        .symbol_name	= "inet_csk_clone_lock",
        .pre_handler = kprobe__inet_csk_clone_lock,
    };

static int kprobe__tcp_child_process(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 1);

    if  (!sock_filter_and_display(sk, 2, "kprobe:tcp_child_process"))
        return 0;

    // pr_debug("\n");
    return 0;
}

const struct kprobe kprobe_hook__tcp_child_process = 
    {
        .symbol_name	= "tcp_child_process",
        .pre_handler = kprobe__tcp_child_process,
    };

static int kretprobe_inet_csk_accept(struct kretprobe_instance *ri, struct pt_regs *regs)
{
    struct sock* newsk = (struct sock*)regs_return_value(regs);

    if  (!sock_filter_and_display(newsk, 2, "kretprobe:inet_csk_accept"))
        return 0;

    pr_debug("\n");
    return 0;
}

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



static int kprobe__tcp_close(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);

    if  (!sock_filter_and_display(sk, 2, "kprobe:tcp_close"))
        return 0;

    // pr_debug("\n");
    return 0;
}


// static int kretprobe_entry__tcp_close(struct kretprobe_instance *ri, struct pt_regs *regs)
// {
//     struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);
//     struct kretprobe_tcp_common_ctx* ctx = (struct kretprobe_tcp_common_ctx*)ri->data;
//     ctx->sk = NULL;

//     if  (!sock_filter_and_display(sk, 2, "kprobe:tcp_close"))
//         return 0;

//     ctx->sk = sk;

//     // pr_debug("\n");
//     return 0;
// }


// static int kretprobe__tcp_close(struct kretprobe_instance *ri, struct pt_regs *regs)
// {
//     struct kretprobe_tcp_common_ctx* ctx = (struct kretprobe_tcp_common_ctx*)ri->data;
//     struct sock* sk = ctx->sk;

//     if  (!sk)
//         return 0;

//     // sock_common_display(sk, "kretprobe:tcp_close");

//     // for close may destroy the sock, so just print sk addr.
//     pr_debug("%-32s  sock: 0x%lx\n", "kretprobe:tcp_close", (unsigned long)sk);

//     pr_debug("\n");
//     return 0;
// }

// const struct kretprobe kretprobe_hook__tcp_close = {
//     .kp = {
//         .symbol_name = "tcp_close",
//     },
//     .entry_handler = kretprobe_entry__tcp_close,
//     .handler = kretprobe__tcp_close,
//     .data_size = sizeof(struct kretprobe_tcp_common_ctx),
//     .maxactive = 64,
// };






static int kprobe__tcp_fin(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);

    if  (!sock_filter_and_display(sk, 2, "kprobe:tcp_fin"))
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


// reset ----------------------------------------------------------


static int kprobe__tcp_reset(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);

    if  (!sock_filter_and_display(sk, 2, "kprobe:tcp_reset"))
        return 0;

    // pr_debug("\n");
    return 0;
}

static void trace__tcp_receive_reset(struct sock* sk)
{
    if  (!sock_filter_and_display(sk, 2, "trace:tcp_receive_reset"))
        return;

    // pr_debug("\n");
}


static int kprobe__tcp_v4_send_reset(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);

    if  (!sock_filter_and_display(sk, 2, "kprobe:tcp_v4_send_reset"))
        return 0;

    // pr_debug("\n");
    return 0;
}

static void trace__tcp_send_reset(const struct sock *sk, const struct sk_buff *skb)
{
    if  (!sock_filter_and_display(sk, 2, "trace:tcp_send_reset"))
        return;

    // pr_debug("\n");
}


// recv and send ---------------------------------------------------------------

static int kprobe__tcp_rcv_established(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);

    if  (!sock_filter_and_display(sk, 3, "kprobe:tcp_rcv_established"))
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


static int kprobe__tcp_write_xmit(struct kprobe *p, struct pt_regs *regs)
{
    struct sock* sk = (struct sock*)x86_64_get_regs_arg(regs, 0);

    if  (!sock_filter_and_display(sk, 3, "kprobe:tcp_write_xmit"))
        return 0;

    // pr_debug("\n");
    return 0;
}




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


// tcp_poll -------------------------------------------------------


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


// module init -----------------------------------------------------

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


#define kprobe_num 28

static struct kprobe kprobes[kprobe_num] = {
    // kprobe_hook__tcp_v4_do_rcv,   
    // kprobe_hook__tcp_rcv_state_process,
    // {
    //     .symbol_name	= "tcp_conn_request",
    //     .pre_handler = kprobe__tcp_conn_request,
    // },
    // kprobe_hook__tcp_conn_request,
    // {
    //     .symbol_name	= "tcp_v4_send_synack",
    //     .pre_handler = kprobe__tcp_v4_send_synack,
    // },
    kprobe_hook__tcp_v4_send_synack,
    // {
    //     .symbol_name	= "tcp_check_req",
    //     .pre_handler = kprobe__tcp_check_req,
    // },
    kprobe_hook__tcp_check_req,
    // {
    //     .symbol_name	= "tcp_v4_syn_recv_sock",
    //     .pre_handler = kprobe__tcp_v4_syn_recv_sock,
    // },
    kprobe_hook__tcp_v4_syn_recv_sock,
    // {
    //     .symbol_name	= "tcp_create_openreq_child",
    //     .pre_handler = kprobe__tcp_create_openreq_child,
    // },
    kprobe_hook__tcp_create_openreq_child,
    // {
    //     .symbol_name	= "inet_csk_clone_lock",
    //     .pre_handler = kprobe__inet_csk_clone_lock,
    // },
    kprobe_hook__inet_csk_clone_lock,
    // {
    //     .symbol_name	= "tcp_child_process",
    //     .pre_handler = kprobe__tcp_child_process,
    // },
    kprobe_hook__tcp_child_process,
    {
        .symbol_name	= "tcp_set_state",
        .pre_handler = kprobe__tcp_set_state,
    },
    // {
    //     .symbol_name	= "tcp_v4_connect",
    //     .pre_handler = kprobe__tcp_v4_connect,
    // },
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
    // {
    //     .symbol_name	= "tcp_close",
    //     .pre_handler = kprobe__tcp_close,
    // },
    {
        .symbol_name	= "tcp_fin",
        .pre_handler = kprobe__tcp_fin,
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
        .symbol_name	= "tcp_timewait_state_process",
        .pre_handler = kprobe__tcp_timewait_state_process,
    },
    {
        .symbol_name	= "tcp_done",
        .pre_handler = kprobe__tcp_done,
    },
    {
        .symbol_name	= "inet_csk_destroy_sock",
        .pre_handler = kprobe__inet_csk_destroy_sock,
    },
    {
        .symbol_name	= "tcp_reset",
        .pre_handler = kprobe__tcp_reset,
    },
    {
        .symbol_name	= "tcp_v4_send_reset",
        .pre_handler = kprobe__tcp_v4_send_reset,
    },
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
    // {
    //     .symbol_name	= "tcp_recvmsg",
    //     .pre_handler = kprobe__tcp_recvmsg,
    // },
    // {
    //     .symbol_name	= "tcp_sendmsg",
    //     .pre_handler = kprobe__tcp_sendmsg,
    // },
    // kprobe_hook__tcp_sendmsg,
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
};



#define kretprobe_num 6

static struct kretprobe kretprobes[kretprobe_num] = {
    {
        .kp = {
            .symbol_name = "inet_csk_accept",
        },
	    .handler = kretprobe_inet_csk_accept,
	    .maxactive = 64,
    },
    {
        .kp = {
            .symbol_name = "tcp_v4_syn_recv_sock",
        },
	    .handler = kretprobe__tcp_v4_syn_recv_sock,
	    .maxactive = 64,
    },
    // {
    //     .kp = {
    //         .symbol_name = "tcp_rcv_state_process",
    //     },
    //     .entry_handler = kretprobe_entry__tcp_rcv_state_process,
	//     .handler = kretprobe__tcp_rcv_state_process,
    //     .data_size = sizeof(struct tcp_rcv_state_process__ctx),
	//     .maxactive = 64,
    // },
    kretprobe_hook__tcp_rcv_state_process,
    kretprobe_hook__tcp_v4_do_rcv,
    kretprobe_hook__tcp_sendmsg,
    kretprobe_hook__tcp_recvmsg,
    // kretprobe_hook__tcp_close,
};





static int __init tcp_trace_init(void)
{
    int ret;
    pr_debug("tcp_trace_init begin\n");

    init_sock_config_from_param();
    sock_filter_config_display(&sock_config);


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

    ret = tcp_trace_conn_init();
    if  (ret < 0)
    {   pr_warn("tcp_trace_conn_init failed\n");
    }

    ret = tcp_trace_data_init();
    if  (ret < 0)
    {   pr_warn("tcp_trace_data_init failed\n");
    }

    pr_debug("tcp_trace_init end\n");
    pr_debug("\n");
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
    
    tcp_trace_conn_exit();

    tcp_trace_data_exit();

    pr_debug("tcp_trace_exit end\n");
    pr_debug("------------------------------------------------------------------\n");
}

module_init(tcp_trace_init)
module_exit(tcp_trace_exit)
MODULE_LICENSE("GPL");
