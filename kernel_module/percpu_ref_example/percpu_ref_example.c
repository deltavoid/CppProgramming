

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>


#include <linux/percpu-refcount.h>
#include <linux/delay.h>



static struct percpu_ref	cnt;



static void example_destroy(struct percpu_ref *ref)
{
    pr_debug("example_put: 1, ref: %ld", atomic_long_read(&ref->count));
    // percpu_ref_kill(ref);

    // pr_debug("example_put: 2, ref: %ld", atomic_long_read(&ref->count));
	// percpu_ref_put(ref);

    pr_debug("example_put: 3, ref: %ld", atomic_long_read(&ref->count));
}


// module init ----------------------------------------------------------

static int __init percpu_ref_example_init(void)
{
    pr_info("percpu_ref_example_init begin\n");
    pr_info("percpu_ref_example_init: 1");

    if  (percpu_ref_init(&cnt, example_destroy, 0, GFP_KERNEL) < 0)
    {
        pr_warn("percpu_ref_init err");
        goto percpu_init_err;
    }


    // pr_info("percpu_ref_example_init: 2");
    pr_info("percpu_ref_example_init: 2, cnt: %ld", atomic_long_read(&cnt.count));
    percpu_ref_get(&cnt);


    // pr_info("percpu_ref_example_init: 3");
    pr_info("percpu_ref_example_init: 3, cnt: %ld", atomic_long_read(&cnt.count));
    percpu_ref_put(&cnt);


    // pr_info("percpu_ref_example_init: 4");
    pr_info("percpu_ref_example_init: 4, cnt: %ld", atomic_long_read(&cnt.count));
    percpu_ref_kill(&cnt);
    
    


    pr_info("percpu_ref_example_init: 5, cnt: %ld", atomic_long_read(&cnt.count));


    msleep(1000);
    pr_info("percpu_ref_example_init: 6, cnt: %ld", atomic_long_read(&cnt.count));

    pr_info("percpu_ref_example_init end\n");
    return 0;

percpu_init_err:

    return -1;
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
