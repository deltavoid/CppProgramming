#ifndef HELLO_IMPL_H
#define HELLO_IMPL_H


#include "hello.h"


class HelloImpl : public Hello {
public:

    int _id;

    HelloImpl();
    virtual ~HelloImpl() override;

    virtual void setId(int id) override;
    virtual int getId() override;
    virtual void printId() override;

};




#endif // HELLO_IMPL_H