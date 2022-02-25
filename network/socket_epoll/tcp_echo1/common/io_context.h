#ifndef IO_CONTEXT_H
#define IO_CONTEXT_H

#include <sys/epoll.h>

#include "epoll_handler.h"


class IOContext {
public:

    int epfd;
    volatile bool running;

    struct epoll_event* events;
    

    static const int max_events = 1024;



    IOContext()
    {
        events = new struct epoll_event[max_events];

        epfd = epoll_create1(0);
        if  (epfd < 0)
            perror("epoll_create1 error");

        running = true;

    }

    ~IOContext()
    {
        close(epfd);
        delete[] events;
    }

    void run()
    {
        while (running)
        {
            printf("IOContext::run: ------------------------------------------\n");
            int num = epoll_wait(epfd, events, max_events, -1);
            printf("IOContext::run: epoll_wait ret %d\n", num);

            for (int i = 0; i < num; i++)
            {
                EpollHandler* handler = (EpollHandler*)events[i].data.ptr;

                if  (handler->handle(events[i].events) < 0)
                    delete handler;
            }    
        }
    }

};

#endif // IO_CONTEXT_H
