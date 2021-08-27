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
    int i;

    for (i = 0; i < ctx->init_num; i++)
    {   struct tracepoint_probe_entry* entry = &ctx->entries[i];
        int ret = tracepoint_probe_register(entry->tp, entry->probe, entry->priv);
        if  (ret)
            pr_warn("trace_%s probe failed\n", entry->name);

    }

    return 0;
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
