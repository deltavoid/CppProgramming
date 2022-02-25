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



const int max_conn = 1024;
const int max_events = max_conn;
// const int max_pkt_size = 64;// 1024;
const int max_pkt_size = 4096;



class EpollHandler
{public:
    virtual int handle(uint32_t events) { return -1;}
    virtual ~EpollHandler() {}
};



struct sockaddr_in local_addr;
int thread_num = 1;
int conn_num = 1;
int pkt_size = 64;
int duration = 10;


int parse_args(int argc, char** argv)
{
    if  (argc < 2)
    {   printf("usage: %s <local port> [<threads>]\n", argv[0]);
        return -1;
    }

    int port_ = 0;
    if  (sscanf(argv[1], "%d", &port_) < 0)
    {   perror("bad port");
        return -1;
    }
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = INADDR_ANY;
    local_addr.sin_port = htons((short)port_);
    bzero(&local_addr.sin_zero, sizeof(local_addr.sin_zero));

    if  (argc > 2)
    {   if  (sscanf(argv[2], "%d", &thread_num) < 0)
        {   perror("bad thread_num");
            return -1;
        }
    }

    printf("local_port: %d, thread_num: %d\n", port_, thread_num);
    return 0;
}


class RingBuffer {
public:
    uint64_t f, p;
    char* buf;

    RingBuffer()
    {
        f = p = 0;
        buf = new char[max_pkt_size];
    }

    ~RingBuffer()
    {
        delete buf;
    }

    int get_blank_size()
    {
        return max_pkt_size - 1 - (p - f);
    }

    int get_exist_size()
    {
        return  p - f;
    }

    int get_blank_iovec(struct iovec* iov, size_t* iov_len_p)
    {
        size_t& iov_len = *iov_len_p;
        int mf = f % max_pkt_size, mp = p % max_pkt_size;
        if  (mp < mf)
        {   iov[0].iov_base = buf + mp;
            iov[0].iov_len = mf - mp - 1;
            iov_len = 1;
        }
        else // mf <= mp
        {
            if  (mf == 0)
            {
                iov[0].iov_base = buf + mp;
                iov[0].iov_len = max_pkt_size - mp - 1;
                iov_len = 1;
            }
            else if  (mf == 1)
            {
                iov[0].iov_base = buf + mp;
                iov[0].iov_len = max_pkt_size - mp;
                iov_len = 1;
            }
            else
            {
                iov[0].iov_base = buf + mp;
                iov[0].iov_len = max_pkt_size - mp;
                iov[1].iov_base = buf;
                iov[1].iov_len = mf - 1;
                iov_len = 2;
            }
        }

        return 0;
    }

    int get_exist_iovec(struct iovec* iov, size_t* iov_len_p)
    {
        size_t& iov_len = *iov_len_p;
        int mf = f % max_pkt_size, mp = p % max_pkt_size;
        if  (mf <= mp)
        {   iov[0].iov_base = buf + mf;
            iov[0].iov_len = mp - mf;
            iov_len = 1;
        }
        else
        {   if  (mp == 0)
            {   iov[0].iov_base = buf + mf;
                iov[0].iov_len = max_pkt_size - mf;
                iov_len = 1;
            }
            else
            {   iov[0].iov_base = buf + mf;
                iov[0].iov_len = max_pkt_size - mf;
                iov[1].iov_base = buf;
                iov[1].iov_len = mp;
                iov_len = 2;
            }
        }

        return 0;
    }

    void add_blank(int v)
    {
        f += v;
    }

    void add_exist(int v)
    {
        p += v;
    }
};


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

class Acceptor : public EpollHandler {
public:
    int fd;
    int epfd;

    Acceptor(struct sockaddr_in* local_addr, int epfd)
        : epfd(epfd)
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

        Connection* conn = new Connection(new_fd, epfd);
        return 0;
    }
};


class EventLoop {
public:
    int epfd;
    volatile bool running;
    // std::unique_ptr<std::thread> thread;

    struct epoll_event* events;

    EventLoop()
    {
        events = new struct epoll_event[max_events];
    
        epfd = epoll_create1(0);
        if  (epfd < 0)
            perror("epoll_create1 error");


        running = true;
        // thread = std::make_unique<std::thread>( [this]() {  this->run();} );
    }

    ~EventLoop()
    {
        // thread->join();

        close(epfd);
        delete[] events;
    }

    void run()
    {
        while (running)
        {
            printf("EventLoop::run: ------------------------------------------\n");
            int num = epoll_wait(epfd, events, max_events, -1);
            printf("EventLoop::run: epoll_wait ret %d\n", num);

            for (int i = 0; i < num; i++)
            {
                EpollHandler* handler = (EpollHandler*)events[i].data.ptr;

                if  (handler->handle(events[i].events) < 0)
                    delete handler;
            }    
        }
    }
};


int main(int argc, char** argv)
{
    printf("hello world\n");
    if  (parse_args(argc, argv) < 0)
        return 1;

    EventLoop loop;
    // loop.thread->join();
    // sleep(10);


    Acceptor acceptor(&local_addr, loop.epfd);
    acceptor.listen(10);
    // acceptor.handle(EPOLLIN); 

    loop.run();


    return 0;
}
