

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


bool sock_filter(const struct sock* sk, int func_level)
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


const char* tcp_state_desc[TCP_MAX_STATES] = {
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

void sock_common_display(const struct sock* sk, const char* prefix)
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



























// module init -----------------------------------------------------




#define kprobe_num 0

static struct kprobe kprobes[kprobe_num] = {

    







};



#define kretprobe_num 0

static struct kretprobe kretprobes[kretprobe_num] = {

};





static int __init tcp_trace_init(void)
{
    int ret;
    pr_debug("tcp_trace_init begin\n");

    init_sock_config_from_param();
    sock_filter_config_display(&sock_config);




    // ret = kprobes_init(kprobes, kprobe_num);
    // if  (ret < 0)
    // {   pr_warn("register kprobes failed\n");
    //     goto kprobes_init_failed;
    // }
       
    // ret = kretprobes_init(kretprobes, kretprobe_num);
    // if  (ret < 0)
    // {   pr_warn("register kretprobes failed\n");
    //     goto kretprobes_init_failed;
    // }

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
    // tracepoint_probe_context_unregister_probes(&sched_probes);

    return ret;
}

static void __exit tcp_trace_exit(void)
{
    pr_debug("tcp_trace_exit begin\n");



    // kprobes_exit(kprobes, kprobe_num);

    // kretprobes_exit(kretprobes, kretprobe_num);
    
    tcp_trace_conn_exit();

    tcp_trace_data_exit();

    pr_debug("tcp_trace_exit end\n");
    pr_debug("------------------------------------------------------------------\n");
}

module_init(tcp_trace_init)
module_exit(tcp_trace_exit)
MODULE_LICENSE("GPL");
