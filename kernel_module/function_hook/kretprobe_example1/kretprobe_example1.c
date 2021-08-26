

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>


#define MAX_SYMBOL_LEN	64
static char symbol[MAX_SYMBOL_LEN] = "_do_fork";
module_param_string(symbol, symbol, sizeof(symbol), 0644);



/* kprobe pre_handler: called just before the probed instruction is executed */
static int kprobe_symbol_pre_handler(struct kprobe *p, struct pt_regs *regs)
{
    // x86_64 function call convention
    unsigned long args[6] = {
        regs->di,
        regs->si,
        regs->dx,
        regs->cx,
        regs->r8,
        regs->r9,
    };

    pr_debug("kprobe_symbol_pre_handler: symbol name: %s, symbol addr: 0x%lx, "
            "arg0: %lu, arg1, %lu, arg2: %lu, arg3: %lu, arg4: %lu, arg5: %lu, "
            "preempt_count: 0x%x\n",
            p->symbol_name, (unsigned long)p->addr,
            args[0], args[1], args[2], args[3], args[4], args[5],
            preempt_count());

    /* A dump_stack() here will give a stack backtrace */
    // dump_stack();

    return 0;
}

/* kprobe post_handler: called after the probed instruction is executed */
static void kprobe_symbol_post_handler(struct kprobe *p, struct pt_regs *regs,
                unsigned long flags)
{
    // x86_64 function call convention
    unsigned long args[6] = {
        regs->di,
        regs->si,
        regs->dx,
        regs->cx,
        regs->r8,
        regs->r9,
    };

    pr_debug("kprobe_symbol_post_handler: symbol name: %s, symbol addr: 0x%lx, flags: 0x%lx "
            "arg0: %lu, arg1, %lu, arg2: %lu, arg3: %lu, arg4: %lu, arg5: %lu, "
            "preempt_count: 0x%x\n",
            p->symbol_name, (unsigned long)p->addr, flags,
            args[0], args[1], args[2], args[3], args[4], args[5],
            preempt_count());

}


/*
 * fault_handler: this is called if an exception is generated for any
 * instruction within the pre- or post-handler, or when Kprobes
 * single-steps the probed instruction.
 */
static int kprobe_generic_fault_handler(struct kprobe *p, struct pt_regs *regs, int trapnr)
{
    pr_info("kprobe_generic_fault_handler: symbol name: %s, p->addr = 0x%p, trap #%dn", 
            p->symbol_name, p->addr, trapnr);
    /* Return 0 because we don't handle the fault. */
    return 0;
}



// kprobes_init --------------------------------------------------------------------------------

static int kprobes_init(struct kprobe* kps, int num)
{
    int i, j;
    int ret;

    for (i = 0; i < num; i++)
    {
        ret = register_kprobe(&kps[i]);
        if  (ret < 0)
        {   pr_warn("kprobes_init: register_kprobe failed, i: %d, symbol name: %s\n",
                    i, kps[i].symbol_name);
            goto kprobes_init_failed;
        }
    }

    return 0;

kprobes_init_failed:
    for (j = i - 1; j >= 0; j--)
        unregister_kprobe(&kps[j]);

    return ret;
}

static void kprobes_exit(struct kprobe* kps, int num)
{
    int i;

    for (i = num - 1; i >= 0; i--)
        unregister_kprobe(&kps[i]);
}



// -----------------------------------------------------------------------

#define kprobe_num 1

static struct kprobe kprobes[kprobe_num] = {

    {
        .symbol_name	= symbol,
        .pre_handler = kprobe_symbol_pre_handler,
        .post_handler = kprobe_symbol_post_handler,
        .fault_handler = kprobe_generic_fault_handler,
    },
};



static int __init kprobe_example_init(void)
{
    pr_debug("kprobe_exmaple_init: 1\n");

    if  (kprobes_init(kprobes, kprobe_num) < 0)
        return -1;


    pr_debug("kprobe_exmaple_init: 2\n");
    return 0;
}

static void __exit kprobe_example_exit(void)
{
    pr_debug("kprobe_exmaple_exit: 1\n");

    kprobes_exit(kprobes, kprobe_num);


    pr_debug("kprobe_exmaple_exit: 2\n");
}


module_init(kprobe_example_init)
module_exit(kprobe_example_exit)
MODULE_LICENSE("GPL");
