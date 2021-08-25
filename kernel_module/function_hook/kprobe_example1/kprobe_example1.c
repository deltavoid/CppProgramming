

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
// #ifdef CONFIG_X86
//     pr_info("<%s> pre_handler: p->addr = 0x%p, ip = %lx, flags = 0x%lx\n",
//         p->symbol_name, p->addr, regs->ip, regs->flags);
// #endif
// #ifdef CONFIG_PPC
//     pr_info("<%s> pre_handler: p->addr = 0x%p, nip = 0x%lx, msr = 0x%lx\n",
//         p->symbol_name, p->addr, regs->nip, regs->msr);
// #endif
// #ifdef CONFIG_MIPS
//     pr_info("<%s> pre_handler: p->addr = 0x%p, epc = 0x%lx, status = 0x%lx\n",
//         p->symbol_name, p->addr, regs->cp0_epc, regs->cp0_status);
// #endif
// #ifdef CONFIG_ARM64
//     pr_info("<%s> pre_handler: p->addr = 0x%p, pc = 0x%lx,"
//             " pstate = 0x%lx\n",
//         p->symbol_name, p->addr, (long)regs->pc, (long)regs->pstate);
// #endif
// #ifdef CONFIG_S390
//     pr_info("<%s> pre_handler: p->addr, 0x%p, ip = 0x%lx, flags = 0x%lx\n",
//         p->symbol_name, p->addr, regs->psw.addr, regs->flags);
// #endif

    // x86_64 function call convention
    unsigned long args[6] = {
        regs->di,
        regs->si,
        regs->dx,
        regs->cx,
        regs->r8,
        regs->r9,
    };



    pr_debug("symbol name: %s, symbol addr: 0x%lx, "
            "arg0: %lu, arg1, %lu, arg2: %lu, arg3: %lu, arg4: %lu, arg5: %lu, "
            "preempt_count: 0x%x",
            p->symbol_name, (unsigned long)p->addr,
            args[0], args[1], args[2], args[3], args[4], args[5],
            preempt_count());


    // unsigned preempt_cnt = preempt_count();
    // if  (preempt_cnt)
    //     pr_debug("preempt_count: %x\n", preempt_cnt);    

    /* A dump_stack() here will give a stack backtrace */
    // dump_stack();
    return 0;
}

// /* kprobe post_handler: called after the probed instruction is executed */
// static void kprobe_symbol_post_handler(struct kprobe *p, struct pt_regs *regs,
//                 unsigned long flags)
// {
// #ifdef CONFIG_X86
//     pr_info("<%s> post_handler: p->addr = 0x%p, flags = 0x%lx\n",
//         p->symbol_name, p->addr, regs->flags);
// #endif
// #ifdef CONFIG_PPC
//     pr_info("<%s> post_handler: p->addr = 0x%p, msr = 0x%lx\n",
//         p->symbol_name, p->addr, regs->msr);
// #endif
// #ifdef CONFIG_MIPS
//     pr_info("<%s> post_handler: p->addr = 0x%p, status = 0x%lx\n",
//         p->symbol_name, p->addr, regs->cp0_status);
// #endif
// #ifdef CONFIG_ARM64
//     pr_info("<%s> post_handler: p->addr = 0x%p, pstate = 0x%lx\n",
//         p->symbol_name, p->addr, (long)regs->pstate);
// #endif
// #ifdef CONFIG_S390
//     pr_info("<%s> pre_handler: p->addr, 0x%p, flags = 0x%lx\n",
//         p->symbol_name, p->addr, regs->flags);
// #endif
// }

/*
 * fault_handler: this is called if an exception is generated for any
 * instruction within the pre- or post-handler, or when Kprobes
 * single-steps the probed instruction.
 */
static int kprobe_generic_fault_handler(struct kprobe *p, struct pt_regs *regs, int trapnr)
{
    pr_info("fault_handler: p->addr = 0x%p, trap #%dn", p->addr, trapnr);
    /* Return 0 because we don't handle the fault. */
    return 0;
}


/* For each probe you need to allocate a kprobe structure */
static struct kprobe kp = {
    .symbol_name	= symbol,
    .pre_handler = kprobe_symbol_pre_handler,
    // .post_handler = kprobe_symbol_post_handler,
    .fault_handler = kprobe_generic_fault_handler,
};

static int __init kprobe_init(void)
{
    int ret;
    // kp.pre_handler = handler_pre;
    // // kp.post_handler = handler_post;
    // kp.post_handler = NULL;
    // kp.fault_handler = handler_fault;

    ret = register_kprobe(&kp);
    if (ret < 0) {
        pr_err("register_kprobe failed, returned %d\n", ret);
        return ret;
    }
    pr_info("Planted kprobe at %p\n", kp.addr);
    return 0;
}

static void __exit kprobe_exit(void)
{
    unregister_kprobe(&kp);
    pr_info("kprobe at %p unregistered\n", kp.addr);
}

module_init(kprobe_init)
module_exit(kprobe_exit)
MODULE_LICENSE("GPL");
