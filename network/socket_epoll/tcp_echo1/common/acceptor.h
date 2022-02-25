#ifndef ACCEPTOR_H
#define ACCEPTOR_H


#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>



#include "epoll_handler.h"
#include "connection_factory.h"


class Acceptor : public EpollHandler {
public:
    int fd;
    int epfd;

    ConnectionFactory* conn_factory;

    Acceptor(struct sockaddr_in* local_addr, int epfd, ConnectionFactory* conn_factory)
        : epfd(epfd), conn_factory(conn_factory)
    {
        fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
        if  (fd < 0)
        {   perror("socket error");
        }

        if  (bind(fd, (struct sockaddr*)local_addr, (socklen_t)sizeof(*local_addr)) < 0)
        {   perror("bind error");
        }

        struct epoll_event event;
        event.events = EPOLLIN;
        event.data.ptr = this;
        if  (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event) < 0)
        {   perror("epoll_ctl add error");
        }

    }

    virtual ~Acceptor()
    {
        if  (epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL) < 0)
        {   perror("epoll_ctl del error");
        }

        close(fd);
    }

    int listen(int backlog)
    {
        return ::listen(fd, backlog);
    }

    virtual int handle(uint32_t ev)
    {
        printf("Acceptor::handle, ev: %d\n", ev);

        if  (ev & ~EPOLLIN)
            return -1;

        struct sockaddr_in remote_addr;
        socklen_t addr_len;
        int new_fd = ::accept4(fd, (struct sockaddr*)&remote_addr, &addr_len, SOCK_NONBLOCK);
        if  (new_fd < 0)
        {   if  (errno == EAGAIN || errno == EWOULDBLOCK)
                return 0;
            else
                return -1;
        }

        // Connection* conn = new Connection(new_fd, epfd);
        
        conn_factory->make_connection(new_fd, epfd);
        
        return 0;
    }
};


#endif // ACCEPTOR_H
