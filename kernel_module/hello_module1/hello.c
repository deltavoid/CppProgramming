

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>




// module init ----------------------------------------------------------

static int __init _module_init(void)
{
    pr_info("%s_init begin\n", KBUILD_MODNAME);


    pr_info("%s_init end\n", KBUILD_MODNAME);
    return 0;
}

static void __exit _module_exit(void)
{
    pr_info("%s_exit begin\n", KBUILD_MODNAME);


    pr_info("%s_exit end\n", KBUILD_MODNAME);
    pr_debug("-------------------------------------------------\n");
}


module_init(_module_init);
module_exit(_module_exit);
MODULE_LICENSE("GPL");
