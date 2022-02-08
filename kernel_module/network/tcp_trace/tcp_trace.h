#ifndef TCP_TRACE_H
#define TCP_TRACE_H



#include <linux/sched.h>

#include <linux/tcp.h>
#include <net/tcp.h>



// tcp_trace_filter.c

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

static inline unsigned get_shifted_tid(void)
{
    return current->pid + smp_processor_id();
}

extern const char* tcp_state_desc[TCP_MAX_STATES];



struct sock_filter_config {
    u16 local_port, remote_port; // 0 indicates not filter;
    bool enable_ipv4;
    bool enable_ipv6;
    u32 local_addr_ipv4, remote_addr_ipv4; // 0 indicates not filter;
};


extern void sock_filter_config_display(struct sock_filter_config* config);


// default global configuraiotn
extern struct sock_filter_config sock_config;

// depends on sock_config
extern bool sock_filter(const struct sock* sk, int func_level);
extern void sock_common_display(const struct sock* sk, const char* prefix);
extern bool sock_filter_and_display(const struct sock* sk, int func_level, const char* prefix);


extern void tcp_recv_skb_display(struct sk_buff* skb);


// typedef struct sock* sock_pointer;
struct kretprobe_tcp_common_ctx {
    struct sock* sk;
};



// tcp_trace_conn.c

extern int tcp_trace_conn_init(void);
extern void tcp_trace_conn_exit(void);


// tcp_trace_data.c

extern int tcp_trace_data_init(void);
extern void tcp_trace_data_exit(void);


#endif // TCP_TRACE_H
