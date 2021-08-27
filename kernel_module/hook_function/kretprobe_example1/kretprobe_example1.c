

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/ktime.h>
#include <linux/limits.h>
#include <linux/sched.h>


#define MAX_SYMBOL_LEN	64
static char symbol[MAX_SYMBOL_LEN] = "_do_fork";
module_param_string(symbol, symbol, sizeof(symbol), 0644);



/* per-instance private data */
struct my_data {
	ktime_t entry_stamp;
};



/* Here we use the entry_hanlder to timestamp function entry */
static int kretprobe_symbol_entry_handler(struct kretprobe_instance *ri, struct pt_regs *regs)
{
	struct my_data *data;

	// if (!current->mm)
	// 	return 1;	/* Skip kernel threads */

	data = (struct my_data *)ri->data;
	data->entry_stamp = ktime_get();
	return 0;
}


/*
 * Return-probe handler: Log the return value and duration. Duration may turn
 * out to be zero consistently, depending upon the granularity of time
 * accounting on the platform.
 */
static int kretprobe_symbol_ret_handler(struct kretprobe_instance *ri, struct pt_regs *regs)
{
	unsigned long retval = regs_return_value(regs);
	struct my_data *data = (struct my_data *)ri->data;
	s64 delta;
	ktime_t now;

	now = ktime_get();
	delta = ktime_to_ns(ktime_sub(now, data->entry_stamp));
	pr_info("%s returned %lu and took %lld ns to execute\n",
			symbol, retval, (long long)delta);
	return 0;
}

// kretprobes_init --------------------------------------------------------------------------------

static int kretprobes_init(struct kretprobe* kps, int num)
{
    int i, j;
    int ret;

    for (i = 0; i < num; i++)
    {
        ret = register_kretprobe(&kps[i]);
        if  (ret < 0)
        {   pr_warn("kprobes_init: register_kprobe failed, i: %d, symbol name: %s\n",
                    i, kps[i].kp.symbol_name);
            goto kretprobes_init_failed;
        }
    }

    return 0;

kretprobes_init_failed:
    for (j = i - 1; j >= 0; j--)
        unregister_kretprobe(&kps[j]);

    return ret;
}

static void kretprobes_exit(struct kretprobe* kps, int num)
{
    int i;

    for (i = num - 1; i >= 0; i--)
        unregister_kretprobe(&kps[i]);
}



// -----------------------------------------------------------------------

#define kprobe_num 1

static struct kretprobe kprobes[kprobe_num] = {

    // {
    //     .symbol_name	= symbol,
    //     .pre_handler = kprobe_symbol_pre_handler,
    //     .post_handler = kprobe_symbol_post_handler,
    //     .fault_handler = kprobe_generic_fault_handler,
    // },
    {
        .kp = {
            .symbol_name = symbol,
        },
	    .handler = kretprobe_symbol_ret_handler,
	    .entry_handler = kretprobe_symbol_entry_handler,
	    .data_size = sizeof(struct my_data),
	    .maxactive = 64,
    },
};



static int __init kprobe_example_init(void)
{
    pr_debug("kprobe_exmaple_init: 1\n");

    if  (kretprobes_init(kprobes, kprobe_num) < 0)
        return -1;


    pr_debug("kprobe_exmaple_init: 2\n");
    return 0;
}

static void __exit kprobe_example_exit(void)
{
    pr_debug("kprobe_exmaple_exit: 1\n");

    kretprobes_exit(kprobes, kprobe_num);


    pr_debug("kprobe_exmaple_exit: 2\n");
}


module_init(kprobe_example_init)
module_exit(kprobe_example_exit)
MODULE_LICENSE("GPL");
