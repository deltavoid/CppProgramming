#pragma once 


// struct hello_ops {
//     void (*set_id)(void* data, int id);
//     int (*get_id)(void* data);
//     void (*print_id)(void* data);
// };

struct hello_ops {
    void (*set_id)(struct hello_ops** ops, int id);
    int (*get_id)(struct hello_ops** ops);
    // void (*print_id)(void* data);
};


// struct hello_trait {
//     struct hello_ops* ops;
//     void* data;
// };


static inline void hello_abstract_set_id(struct hello_ops** ops, int id)
{
    (*ops)->set_id(ops, id);
}

static inline int hello_abstract_get_id(struct hello_ops** ops)
{
    return (*ops)->get_id(ops);
}