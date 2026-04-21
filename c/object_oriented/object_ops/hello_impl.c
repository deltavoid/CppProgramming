
#include "hello_impl.h"
#include "container_of.h"

#include <stdio.h>

int hello_impl_init(struct hello_impl* obj, int id)
{
    obj->ops = &hello_impl_ops;
    obj->id = id;

    return 0;
}

// void hello_impl_set_id(void* data, int id)
void hello_impl_set_id(struct hello_ops** ops, int id)
{
    // struct hello_impl* obj = (struct hello_impl*)data;
    struct hello_impl* obj = (struct hello_impl*)container_of(ops, struct hello_impl, ops);


    obj->id = id;
}

// int hello_impl_get_id(void* data)
int hello_impl_get_id(struct hello_ops** ops)
{
    // struct hello_impl* obj = (struct hello_impl*)data;
    struct hello_impl* obj = (struct hello_impl*)container_of(ops, struct hello_impl, ops);

    return obj->id;
}

// void hello_impl_print_id(void* data)
// {
//     // struct hello_impl* obj = (struct hello_impl*)data;

//     // printf("this: 0x%lx, id: %d\n", data, obj->id);
// }








const struct hello_ops hello_impl_ops = {
    .set_id = hello_impl_set_id,
    .get_id = hello_impl_get_id,
    // .print_id = hello_impl_print_id,
};



