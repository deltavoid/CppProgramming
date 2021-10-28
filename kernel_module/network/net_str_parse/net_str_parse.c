

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>


#include <linux/inet.h>


const char* input = "10.16.118.56/24";
__be32 res_net, res_prefix;



static int net_seg_pton(const char* input, __be32* res_net_p, __be32* res_prefix_p)
{
    const char* end = NULL;
    int ret = in4_pton(input, strlen(input), (u8*)res_net_p, '/', &end);

    pr_debug("res_net_p: %pI4\n", res_net_p);
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
