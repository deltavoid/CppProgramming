

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>




// module init ----------------------------------------------------------

static int __init sem_use_init(void)
{
    pr_info("sem_use_init begin\n");


    pr_info("sem_use_init end\n");
    return 0;
}

static void __exit sem_use_exit(void)
{
    pr_info("sem_use_exit begin\n");


    pr_info("sem_use_exit end\n");
    pr_debug("-------------------------------------------------\n");
}


module_init(sem_use_init);
module_exit(sem_use_exit);
MODULE_LICENSE("GPL");
