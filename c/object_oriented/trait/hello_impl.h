#ifndef HELLO_IMPL_H
#define HELLO_IMPL_H

#include "hello_trait.h"


struct hello_impl {

    int id;

};

extern void hello_impl_set_id(void* data, int id);
extern int hello_impl_get_id(void* data);
extern void hello_impl_print_id(void* data);


extern const struct hello_ops hello_impl_ops;






#endif // HELLO_IMPL_H

