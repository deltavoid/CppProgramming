

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>

#include <linux/miscdevice.h>
#include <linux/inetdevice.h>
#include <linux/mm.h>
#include <linux/vmalloc.h>
#include <linux/file.h>



// build a char device for user to mmap, map two areas, one control area, and one queue area
#define EXAMPLE_CHAR_DEVICE_PATH "/dev/example_mmap_char_device"


static int example_dev_open(struct inode *inode, struct file *filp)
{
    pr_debug("example_dev_open");
    filp->private_data = NULL;

    return 0;
}

static int example_dev_release(struct inode *inode, struct file *filp)
{
    pr_debug("example_dev_release");



    return 0;
}

static int example_dev_mmap(struct file *filp, struct vm_area_struct *vma)
{
    pr_debug("example_dev_mmap");

    return 0;
}

static long example_dev_ioctl(struct file *filp, unsigned int cmd,
                              unsigned long arg)
{
    pr_debug("example_dev_ioctl");

    return 0;
}


static unsigned int example_dev_poll(struct file *filp, struct poll_table_struct *wait)
{
    pr_debug("example_dev_poll");
    /* Never report ready, so that this device can be added to poll/select
     * without affecting results. */

    return 0;
}




static struct file_operations example_char_device_fops = {
    .owner          = THIS_MODULE,
    .open           = example_dev_open,
    .release        = example_dev_release,
    .mmap           = example_dev_mmap,
    .unlocked_ioctl = example_dev_ioctl,
    .poll           = example_dev_poll,
};




static struct miscdevice example_char_device;




// module init ----------------------------------------------------------

static int __init example_module_init(void)
{
    int ret;
    pr_info("example_module_init begin\n");



    /* Create /dev/instanta_socket device */
    example_char_device.minor = MISC_DYNAMIC_MINOR;
    example_char_device.name  = EXAMPLE_CHAR_DEVICE_PATH;
    example_char_device.fops = &example_char_device_fops;
    example_char_device.mode = S_IFCHR|S_IRUGO|S_IWUGO; // 支持非root账户下运行用户态协议栈
    ret = misc_register(&example_char_device);
    if (ret != 0)
    {
        pr_err("Failed to register instanta device: %d\n", ret);
        return -1;
    }



    pr_info("example_module_init end\n");
    return 0;
}

static void __exit example_module_exit(void)
{
    pr_info("example_module_exit begin\n");

    misc_deregister(&example_char_device);



    pr_info("example_module_exit end\n");
    pr_debug("-------------------------------------------------\n");
}


module_init(example_module_init);
module_exit(example_module_exit);
MODULE_LICENSE("GPL");
