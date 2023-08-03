

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>




// module init ----------------------------------------------------------

static int __init char_device_mmap_init(void)
{
    pr_info("char_device_mmap_init begin\n");


    pr_info("char_device_mmap_init end\n");
    return 0;
}

static void __exit char_device_mmap_exit(void)
{
    pr_info("char_device_mmap_exit begin\n");


    pr_info("char_device_mmap_exit end\n");
    pr_debug("-------------------------------------------------\n");
}


module_init(char_device_mmap_init);
module_exit(char_device_mmap_exit);
MODULE_LICENSE("GPL");
