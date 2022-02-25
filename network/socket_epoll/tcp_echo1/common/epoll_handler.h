#ifndef EPOLL_HANDLER_H
#define EPOLL_HANDLER_H


#include <stdint.h>


class EpollHandler{
public:
    virtual int handle(uint32_t events) { return -1;}
    virtual ~EpollHandler() {}
};





#endif // EPOLL_HANDLER_H
