

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

#include "char_device_mmap.h"


// build a char device for user to mmap, map two areas, one control area, and one queue area



int example_mmap_char_dev_meta_init(struct example_mmap_char_dev_meta_t* data)
{
    data->ctrl = vmalloc_user(sizeof( *(data->ctrl)));
    if  (data->ctrl == NULL)
    {
        pr_err("malloc ctrl failed\n");
    }
    pr_info("data->ctrl: %lx\n", (long)data->ctrl);

    data->ctrl->queue_size = EXAMPLE_CHAR_DEVICE_QUEUE_SIZE;

    data->queue = vmalloc_user(EXAMPLE_CHAR_DEVICE_QUEUE_SIZE);
    if  (data->queue == NULL)
    {
        pr_err("malloc ctrl failed\n");
    }
    pr_info("data->queue: %lx\n", (long)data->queue);

    return 0;
}

void example_mmap_char_dev_meta_exit(struct example_mmap_char_dev_meta_t* data)
{

    vfree(data->queue);
    vfree(data->ctrl);

}

struct example_mmap_char_dev_meta_t example_mmap_dev;





static int example_dev_open(struct inode *inode, struct file *filp)
{
    pr_debug("example_dev_open\n");
    filp->private_data = NULL;

    return 0;
}

static int example_dev_release(struct inode *inode, struct file *filp)
{
    pr_debug("example_dev_release\n");



    return 0;
}

static int example_dev_mmap(struct file *filp, struct vm_area_struct *vma)
{
    pr_debug("example_dev_mmap\n");


    if  (vma->vm_pgoff >= EXAMPLE_CHAR_DEVICE_MMAP_QUEUE_OFFSET / PAGE_SIZE)
    {
        // mmap queue area
        pr_info("mmap queue\n");

        return remap_vmalloc_range(vma, example_mmap_dev.queue, vma->vm_pgoff);
    }
    else
    {
        // mmap ctrl area
        pr_info("mmap ctrl\n");

        return remap_vmalloc_range(vma, example_mmap_dev.ctrl, vma->vm_pgoff);
    }

    return 0;
}

static long example_dev_ioctl(struct file *filp, unsigned int cmd,
                              unsigned long arg)
{
    pr_debug("example_dev_ioctl\n");

    return 0;
}


static unsigned int example_dev_poll(struct file *filp, struct poll_table_struct *wait)
{
    pr_debug("example_dev_poll\n");
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

    example_mmap_char_dev_meta_init(&example_mmap_dev);

    /* Create /dev/instanta_socket device */
    example_char_device.minor = MISC_DYNAMIC_MINOR;
    example_char_device.name  = EXAMPLE_CHAR_DEVICE_NAME;
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

    example_mmap_char_dev_meta_exit(&example_mmap_dev);



    pr_info("example_module_exit end\n");
    pr_debug("-------------------------------------------------\n");
}


module_init(example_module_init);
module_exit(example_module_exit);
MODULE_LICENSE("GPL");
