

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>


#include <linux/inet.h>


const char* input = "10.16.118.56/24";
// __be32 res_net, res_prefix;

unsigned res_net, res_prefix; // host byte sequence


static int net_seg_pton(const char* input, unsigned* res_net_p, unsigned* res_prefix_p)
{
    __be32 res_net_net_seq, res_prefixed_net_net_seq;
    unsigned res_net_host_seq, res_prefix_host_seq, res_prefixed_net_host_seq;
    int prefix_num = 0;
    int ret;
    const char* end = NULL;


    int len = strlen(input);
    pr_debug("len: %d\n", len);

    ret = in4_pton(input, len, (u8*)&res_net_net_seq, '/', &end);
    pr_debug("res_net_p: %pI4\n", &res_net_net_seq);
    
    res_net_host_seq = ntohl(res_net_net_seq);


    sscanf(end + 1, "%d", &prefix_num);
    pr_debug("prefix_num: %d\n", prefix_num);

    res_prefix_host_seq = ((1 << prefix_num) - 1) << (32 - prefix_num);
    pr_debug("prefix: %x\n", res_prefix_host_seq);

    *res_net_p = res_net_host_seq;
    *res_prefix_p = res_prefix_host_seq;

    res_prefixed_net_host_seq = res_net_host_seq & res_prefix_host_seq;
    res_prefixed_net_net_seq = htonl(res_prefixed_net_host_seq);

    pr_debug("prefixed_net: %pI4\n", &res_prefixed_net_net_seq);
}




// module init ----------------------------------------------------------

static int __init hello_init(void)
{
    pr_info("hello_init begin\n");


    net_seg_pton(input, &res_net, &res_prefix);



    pr_info("hello_init end\n");
    return 0;
}

static void __exit hello_exit(void)
{
    pr_info("hello_exit begin\n");


    pr_info("hello_exit end\n");
    pr_debug("-------------------------------------------------\n");
}


module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
