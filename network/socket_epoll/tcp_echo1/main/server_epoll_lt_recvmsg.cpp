#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>

#include <thread>
#include <memory>
#include <vector>
#include <map>


#include <unistd.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>



#include "common/config.h"
#include "common/io_context.h"
#include "common/connection_factory.h"
#include "common/ring_buffer.h"
#include "common/acceptor.h"





class Connection : public EpollHandler {
public:
    int fd;
    int epfd;

    RingBuffer buf;

    // bool enable_epollout, enable_epollin;
    uint32_t epoll_event;

    Connection(int fd, int epfd)
        : fd(fd), epfd(epfd)
    {
        printf("Connection::Connection: fd: %d, epfd: %d\n", fd, epfd);

        // enable_epollout = false;
        // enable_epollin = true;
        epoll_event = EPOLLIN;

        struct epoll_event event;
        // event.events = EPOLLIN;
        event.events = epoll_event;
        event.data.ptr = this;
        if  (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event) < 0)
        {   perror("epoll_ctl add error");
        }
    }

    virtual ~Connection()
    {
        if  (epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL) < 0)
            perror("epoll_ctl del error");
        
        close(fd);
        printf("fd %d closed\n", fd);
    }

    bool enable_epollout()
    {
        return epoll_event & EPOLLOUT;
    }

    void set_epollout(bool flag)
    {
        if  (epoll_event & EPOLLOUT)
        {   if  (flag)
                return;
            else
                epoll_event &= ~EPOLLOUT;
        }
        else
        {   if  (flag)
                epoll_event |= EPOLLOUT;
            else
                return;
        }

        printf("Connection::set_epollout: to %d\n", flag);

        struct epoll_event event;
        event.events = epoll_event;
        event.data.ptr = this;
        if  (epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &event) < 0)
            perror("epoll_ctl error");
    }


    bool enable_epollin()
    {
        return epoll_event & EPOLLIN;
    }

    void set_epollin(bool flag)
    {
        if  (epoll_event & EPOLLIN)
        {   if  (flag)
                return;
            else
                epoll_event &= ~EPOLLIN;
        }
        else
        {   if  (flag)
                epoll_event |= EPOLLIN;
            else
                return;
        }

        printf("Connection::set_epollin: to %d\n", flag);

        struct epoll_event event;
        event.events = epoll_event;
        event.data.ptr = this;
        if  (epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &event) < 0)
            perror("epoll_ctl error");
    }


    int recv_buf()
    {
        // if  (buf.get_blank_size() <= 0)
        //     return 0;
        int to_read = buf.get_blank_size();
        if  (to_read < 0)
        {   return -1;
        }
        else if  (to_read == 0)
        {   set_epollin(false);
            return 0;
        }

        struct iovec iov[2];
        size_t iov_len = 0;
        if  (buf.get_blank_iovec(iov, &iov_len) < 0)
            return 0;

        struct msghdr msg = {
            .msg_name = NULL,
            .msg_namelen = 0,
            .msg_iov = iov,
            .msg_iovlen = iov_len,
            .msg_control = NULL,
            .msg_controllen = 0
        };
        int recv_len = recvmsg(fd, &msg, 0);
        printf("Connection::recv_buf, recv_len: %d\n", recv_len);
        // if  (recv_len <= 0)
        //     return -1;
        if  (recv_len == 0)
        {   return -1;
        }
        else if  (recv_len < 0)
        {   if  (errno == EAGAIN || errno == EWOULDBLOCK)
                recv_len = 0;
            else
                return -1;
        }

        buf.add_exist(recv_len);
        // if  (recv_len > 0)
        //     set_epollout(true);

        
        int to_read1 = buf.get_blank_size();
        if  (to_read1 < 0)
        {   return -1;
        }
        else if  (to_read1 == 0)
        {   set_epollin(false);
        }
        else
        {   set_epollin(true);
        }
        
        printf("Connection::recv_buf, f: %llu, p: %llu\n", buf.f, buf.p);
        return recv_len;
    }

    // int recv_buf_native()
    // {
    //     if  (buf.get_blank_size() <= 0)
    //         return 0;

    //     struct iovec iov[2];
    //     size_t iov_len = 0;
    //     if  (buf.get_blank_iovec(iov, &iov_len) < 0)
    //         return 0;

    //     int ret = 0;
    //     for (int i = 0; i < iov_len; i++)
    //     {
    //         int recv_len = ::recv(fd, iov[i].iov_base, iov[i].iov_len, 0);
    //         printf("Connection::recv_buf_native, recv_len: %d, iov[%d].len: %d\n", recv_len, i, iov[i].iov_len);

    //         if  (recv_len == 0)
    //             return -1;
            
    //         if  (recv_len < 0)
    //         {   if  (errno == EAGAIN || errno == EWOULDBLOCK)
    //                 recv_len = 0;
    //             else
    //                 return -1;
    //         }

    //         buf.add_exist(recv_len);
    //         printf("Connection::recv_buf_native, f: %llu, p: %llu\n", buf.f, buf.p);
    //         ret += recv_len;
    //         if  (recv_len < iov[i].iov_len)
    //             break;
    //     }

    //     return ret;
    // }

    int send_buf()
    {
        int to_send = buf.get_exist_size();
        if  (to_send < 0)
        {   return -1;
        }
        else if  (to_send == 0)
        {   set_epollout(false);
            return 0;
        }
        // if  (to_send <= 0)
        //     return 0;
        
        struct iovec iov[2];
        size_t iov_len = 0;
        buf.get_exist_iovec(iov, &iov_len);

        struct msghdr msg = {
            .msg_name = NULL,
            .msg_namelen = 0,
            .msg_iov = iov,
            .msg_iovlen = iov_len,
            .msg_control = NULL,
            .msg_controllen = 0
        };
        int send_len = sendmsg(fd, &msg, 0);
        printf("Connection::send_buf, send_len: %d\n", send_len);
                
        if  (send_len < 0)
        {
            if  (errno == EAGAIN || errno == EWOULDBLOCK)
            {   send_len = 0;
            }
            else
                return -1;
        }

        buf.add_blank(send_len);
        // if  (send_len > 0)
        //     set_epollin(true);


        // if  (send_len == to_send)
        // {
        //     // if  (enable_epollout())
        //         set_epollout(false);
        // }
        // else
        // {
        //     // if  (!enable_epollout())
        //         set_epollout(true);
        // }

        int to_send1 = buf.get_exist_size();
        if  (to_send1 < 0)
        {   return -1;
        }
        else if  (to_send1 == 0)
        {   set_epollout(false);
            // return 0;
        }
        else
        {   set_epollout(true);
        }

        printf("Connection::send_buf, f: %llu, p: %llu\n", buf.f, buf.p);
        return send_len;
    }

    // int send_buf_native()
    // {
    //     int to_send = buf.get_exist_size();
    //     if  (to_send <= 0)
    //         return 0;
        
    //     struct iovec iov[2];
    //     size_t iov_len = 0;
    //     buf.get_exist_iovec(iov, &iov_len);

    //     int have_send = 0;
    //     for (int i = 0; i < iov_len; i++)
    //     {
    //         int send_len = send(fd, iov[i].iov_base, iov[i].iov_len, 0);
    //         printf("Connection::send_buf_native, send_len: %d, iov[%d].len: %d\n", send_len, i, iov[i].iov_len);

    //         if  (send_len < 0)
    //         {   if  (errno == EAGAIN || errno == EWOULDBLOCK)
    //                 send_len = 0;
    //             else
    //                 return -1;
    //         }

    //         buf.add_blank(send_len);
    //         printf("Connection::send_buf_native, f: %llu, p: %llu\n", buf.f, buf.p);
    //         have_send += send_len;

    //         if  (have_send == to_send)
    //         {   if  (enable_epollout())
    //                 set_epollout(false);
    //         }
    //         else if  (send_len < iov[i].iov_len)
    //         {   if  (!enable_epollout())
    //                 set_epollout(true);
    //             break;
    //         }
    //     }

    //     return have_send;
    // }

    int handle_read()
    {
        int recv_len = recv_buf();
        if  (recv_len < 0)
            return -1;
        else if (recv_len > 0)
        {
            int send_len = send_buf();
            if  (send_len < 0)
                return -1;
            else if  (send_len > 0)
                set_epollin(true);
        }

        return 0;
    }

    int handle_write()
    {
        int send_len = send_buf();

        if  (send_len < 0)
            return -1;
        else if  (send_len > 0)
            set_epollin(true);

        return 0;
    }

    virtual int handle(uint32_t ev)
    {
        int ret = -1;
        printf("Connection::handle: ev: %d\n", ev);
        // return -1;

        if  (ev & ~(EPOLLIN | EPOLLOUT))
            return -1;

        if  (ev & EPOLLOUT)
        {
            ret = handle_write();
            if  (ret < 0)
                return ret;
        }
            
        if  (ev & EPOLLIN)
        {
            ret = handle_read();
            printf("Connection::handle, handle_read: %d\n", ret);
            if  (ret < 0)
                return ret;
        }

        return 0;
    }
};


class ThisConnectionFactory : public ConnectionFactory {
public:
    
    int make_connection(int fd, int epfd)
    {
        Connection* conn = new Connection(fd, epfd);
        return 0;
    }


};



Config config;
ThisConnectionFactory conn_factory;

int main(int argc, char** argv)
{
    printf("hello world\n");
    if  (config.parse_args(argc, argv) < 0)
        return 1;

    // EventLoop loop;
    IOContext io_context;
    // loop.thread->join();
    // sleep(10);


    Acceptor acceptor(&config.local_addr, io_context.epfd, &conn_factory);
    acceptor.listen(10);
    // acceptor.handle(EPOLLIN); 

    // loop.run();
    io_context.run();


    return 0;
}
