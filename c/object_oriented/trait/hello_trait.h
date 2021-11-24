#ifndef HELLO_TRAIT_H
#define HELLO_TRAIT_H



struct hello_ops {
    void (*set_id)(void* data, int id);
    int (*get_id)(void* data);
    void (*print_id)(void* data);
};

struct hello_trait {
    struct hello_ops* ops;
    void* data;
};



static inline void hello_trait_set_id(struct hello_trait* obj, int id)
{
    obj->ops->set_id(obj->data, id);
}

static inline int hello_trait_get_id(struct hello_trait* obj)
{
    return obj->ops->get_id(obj->data);
}

static inline void hello_trait_print_id(struct hello_trait* obj)
{
    obj->ops->print_id(obj->data);
}


#endif // HELLO_TRAIT_H

