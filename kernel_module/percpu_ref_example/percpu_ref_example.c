

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>




// module init ----------------------------------------------------------

static int __init percpu_ref_example_init(void)
{
    pr_info("percpu_ref_example_init begin\n");


    pr_info("percpu_ref_example_init end\n");
    return 0;
}

static void __exit percpu_ref_example_exit(void)
{
    pr_info("percpu_ref_example_exit begin\n");


    pr_info("percpu_ref_example_exit end\n");
    pr_debug("-------------------------------------------------\n");
}


module_init(percpu_ref_example_init);
module_exit(percpu_ref_example_exit);
MODULE_LICENSE("GPL");
