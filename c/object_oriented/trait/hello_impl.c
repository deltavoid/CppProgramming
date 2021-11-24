

#include "hello_impl.h"

#include "stdio.h"



void hello_impl_set_id(void* data, int id)
{
    struct hello_impl* obj = (struct hello_impl*)data;

    obj->id = id;
}

int hello_impl_get_id(void* data)
{
    struct hello_impl* obj = (struct hello_impl*)data;

    return obj->id;
}

void hello_impl_print_id(void* data)
{
    struct hello_impl* obj = (struct hello_impl*)data;

    printf("this: 0x%lx, id: %d\n", data, obj->id);
}








const struct hello_ops hello_impl_ops = {
    .set_id = hello_impl_set_id,
    .get_id = hello_impl_get_id,
    .print_id = hello_impl_print_id,
};



