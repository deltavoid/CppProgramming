


#include "hook_func_lib.h"
#include "tcp_trace.h"
















// init -----------------------------------------

#define kprobe_num 0

static struct kprobe kprobes[kprobe_num] = {

    // kprobe_hook__tcp_conn_request,


};


#define kretprobe_num 0

static struct kretprobe kretprobes[kretprobe_num] = {

    // kretprobe_hook__tcp_close,

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

