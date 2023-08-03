

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>`

#include <linux/miscdevice.h>
#include <linux/inetdevice.h>
#include <linux/mm.h>
#include <linux/vmalloc.h>
#include <linux/file.h>



// build a char device for user to mmap, map two areas, one control area, and one queue area



static int example_dev_open(struct inode *inode, struct file *filp)
{
    filp->private_data = NULL;

    return 0;
}

static int example_dev_release(struct inode *inode, struct file *filp)
{
    return 0;

}

static int example_dev_mmap(struct file *filp, struct vm_area_struct *vma)
{
    return 0;

}

static long example_dev_ioctl(struct file *filp, unsigned int cmd,
                              unsigned long arg)
{
    return 0;

}


static unsigned int example_dev_poll(struct file *filp, struct poll_table_struct *wait)
{
    /* Never report ready, so that this device can be added to poll/select
     * without affecting results. */
    return 0;
}




static struct file_operations example_fops = {
    .owner          = THIS_MODULE,
    .open           = example_dev_open,
    .release        = example_dev_release,
    .mmap           = example_dev_mmap,
    .unlocked_ioctl = example_dev_ioctl,
    .poll           = example_dev_poll,
};








// module init ----------------------------------------------------------

static int __init example_module_init(void)
{
    pr_info("example_module_init begin\n");


    pr_info("example_module_init end\n");
    return 0;
}

static void __exit example_module_exit(void)
{
    pr_info("example_module_exit begin\n");


    pr_info("example_module_exit end\n");
    pr_debug("-------------------------------------------------\n");
}


module_init(example_module_init);
module_exit(example_module_exit);
MODULE_LICENSE("GPL");
