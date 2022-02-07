

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


























// module init -----------------------------------------------------





static int __init tcp_trace_init(void)
{
    int ret;
    pr_debug("tcp_trace_init begin\n");

    init_sock_config_from_param();
    sock_filter_config_display(&sock_config);


    ret = tcp_trace_conn_init();
    if  (ret < 0)
    {   pr_warn("tcp_trace_conn_init failed\n");
        goto conn_init_failed;
    }

    ret = tcp_trace_data_init();
    if  (ret < 0)
    {   pr_warn("tcp_trace_data_init failed\n");
        goto data_init_failed;
    }

    pr_debug("tcp_trace_init end\n");
    pr_debug("\n");
    return 0;

data_init_failed:
    tcp_trace_conn_exit();
conn_init_failed:

    return ret;
}

static void __exit tcp_trace_exit(void)
{
    pr_debug("tcp_trace_exit begin\n");

    
    tcp_trace_conn_exit();

    tcp_trace_data_exit();

    pr_debug("tcp_trace_exit end\n");
    pr_debug("------------------------------------------------------------------\n");
}

module_init(tcp_trace_init)
module_exit(tcp_trace_exit)
MODULE_LICENSE("GPL");
