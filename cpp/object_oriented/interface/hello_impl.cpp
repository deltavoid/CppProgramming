#include "hello_impl.h"

#include <cstdio>


HelloImpl::HelloImpl()
{
    _id = 0;
}

HelloImpl::~HelloImpl()
{
    printf("HelloImpl::~HelloImpl\n");
}


void HelloImpl::setId(int id)
{
    _id = id;
}

int HelloImpl::getId()
{
    return _id;
}

void HelloImpl::printId()
{
    printf("this: 0x%lx, id: %d\n", this, _id);
}
