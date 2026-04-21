#pragma once

#include "hello_interface.h"

struct hello_impl {
    struct hello_ops* ops;

    int id;

};

extern int hello_impl_init(struct hello_impl* obj, int id);

extern void hello_impl_set_id(void* data, int id);
extern int hello_impl_get_id(void* data);
extern void hello_impl_print_id(void* data);


extern const struct hello_ops hello_impl_ops;





