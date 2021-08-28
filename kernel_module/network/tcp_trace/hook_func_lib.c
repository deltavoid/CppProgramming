#include "hook_func_lib.h"





// tracepoint_probe_context ----------------------------------------

static void tracepoint_lookup(struct tracepoint *tp, void *priv)
{
    int i;
    struct tracepoint_probe_context* ctx = priv;

    if  (ctx->found_num == ctx->init_num)
        return;

    for (i = 0; i < ctx->init_num; i++)
    {   struct tracepoint_probe_entry* entry = &ctx->entries[i];

        if  (entry->tp || strcmp(tp->name, entry->name))
            continue;
        entry->tp = tp;
        ctx->found_num++;
    }
}

int tracepoint_probe_context_find_tracepoints(struct tracepoint_probe_context* ctx)
{
    int i;
    for (i = 0; i < ctx->init_num; i++)
        ctx->entries[i].tp = NULL;
    ctx->found_num = 0;


    for_each_kernel_tracepoint(tracepoint_lookup, ctx);

    if  (ctx->found_num != ctx->init_num)
        return -1;
    return 0;
}

int tracepoint_probe_context_register_probes(struct tracepoint_probe_context* ctx)
{
    int i, j;
    int ret;

    for (i = 0; i < ctx->init_num; i++)
    {   struct tracepoint_probe_entry* entry = &ctx->entries[i];
        ret = tracepoint_probe_register(entry->tp, entry->probe, entry->priv);
        if  (ret)
        {   pr_warn("trace_%s probe failed\n", entry->name);
            goto register_failed;
        }
    }

    return 0;

register_failed:

    for (j = i - 1; j >= 0; j--)
    {   struct tracepoint_probe_entry* entry = &ctx->entries[j];
        tracepoint_probe_unregister(entry->tp, entry->probe, entry->priv);
    }

    tracepoint_synchronize_unregister();

    return ret;
}

void tracepoint_probe_context_unregister_probes(struct tracepoint_probe_context* ctx)
{
    int i;

    for (i = 0; i < ctx->init_num; i++)
    {   struct tracepoint_probe_entry* entry = &ctx->entries[i];
        tracepoint_probe_unregister(entry->tp, entry->probe, entry->priv);
    }

    tracepoint_synchronize_unregister();
}






// kprobes_init --------------------------------------------------------------------------------

int kprobes_init(struct kprobe* kps, int num)
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

void kprobes_exit(struct kprobe* kps, int num)
{
    int i;

    for (i = num - 1; i >= 0; i--)
        unregister_kprobe(&kps[i]);
}



// kretprobes_init --------------------------------------------------------------------------------

int kretprobes_init(struct kretprobe* kps, int num)
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

void kretprobes_exit(struct kretprobe* kps, int num)
{
    int i;

    for (i = num - 1; i >= 0; i--)
        unregister_kretprobe(&kps[i]);
}
