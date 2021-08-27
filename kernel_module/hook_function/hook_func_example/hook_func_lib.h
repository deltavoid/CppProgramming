#ifndef HOOK_FUNC_LIB_H
#define HOOK_FUNC_LIB_H

#include <linux/tracepoint.h>
#include <linux/kprobes.h>




struct tracepoint_probe_entry {
    char* name;
    struct tracepoint *tp;
    void* probe;
    void* priv;
};

#define TRACEPOINT_PROBE_CONTEXT_MAX_NUM 8

struct tracepoint_probe_context {
    struct tracepoint_probe_entry entries[TRACEPOINT_PROBE_CONTEXT_MAX_NUM];
    int init_num;
    int found_num;
};


extern int tracepoint_probe_context_find_tracepoints(struct tracepoint_probe_context* ctx);
extern int tracepoint_probe_context_register_probes(struct tracepoint_probe_context* ctx);
extern void tracepoint_probe_context_unregister_probes(struct tracepoint_probe_context* ctx);


extern int kprobes_init(struct kprobe* kps, int num);
extern void kprobes_exit(struct kprobe* kps, int num);


extern int kretprobes_init(struct kretprobe* kps, int num);
extern void kretprobes_exit(struct kretprobe* kps, int num);


#endif // HOOK_FUNC_LIB_H
