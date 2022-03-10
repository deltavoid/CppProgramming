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
#include <ucontext.h>



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

    // RingBuffer buf;

    enum class WaitStatus {
        running = 0,
        wait_could_recv,
        wait_could_send,
    }wait_status;

    ucontext_t ctx_main, ctx_fnew;

    bool exited;

    static const int buf_size = 64;
    char buffer[buf_size];

    static const int stack_size = 4096;
    char stack[stack_size];


    Connection(int fd, int epfd)
        : fd(fd), epfd(epfd)
    {
        int ret = -1;
        printf("Connection::Connection: fd: %d, epfd: %d\n", fd, epfd);


        struct epoll_event event;
        event.events = EPOLLIN | EPOLLOUT | EPOLLET;
        event.data.ptr = this;
        if  (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event) < 0)
        {   perror("epoll_ctl add error");
        }



        printf("Connection::Connection: end\n");
    }

    virtual ~Connection()
    {
        if  (epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL) < 0)
            perror("epoll_ctl del error");
        
        close(fd);
        printf("fd %d closed\n", fd);
    }


    int start()
    {
        int ret = -1;
        printf("Connection::start: 1\n");


        ret = getcontext(&this->ctx_fnew);
        this->ctx_fnew.uc_stack.ss_sp = &this->stack;
	    this->ctx_fnew.uc_stack.ss_size = this->stack_size;
        this->ctx_fnew.uc_link = &this->ctx_main;
        exited = false;

        printf("Connection::start: 2\n");
        makecontext(&this->ctx_fnew, (void (*)(void)) run, 1, this);

        printf("Connection::start: 3\n");
        wait_status = WaitStatus::running;
        ret = swapcontext(&this->ctx_main, &this->ctx_fnew);
        if  (ret != 0)
        {   perror("swapcontext error at Connection::Connection");
        }


        printf("Connection::start: 4, end\n");
        return 0;
    }


    // int recv_buf()
    // {
    //     if  (buf.get_blank_size() <= 0)
    //     {
    //         printf("error: read buf should not be empty\n");
    //         return -1;
    //     }

    //     // at this stage not support read flow control, just assume the read buf is enough.

    //     struct iovec iov[2];
    //     size_t iov_len = 0;
    //     if  (buf.get_blank_iovec(iov, &iov_len) < 0)
    //         return 0;

    //     struct msghdr msg = {
    //         .msg_name = NULL,
    //         .msg_namelen = 0,
    //         .msg_iov = iov,
    //         .msg_iovlen = iov_len,
    //         .msg_control = NULL,
    //         .msg_controllen = 0
    //     };
    //     int recv_len = recvmsg(fd, &msg, 0);
    //     printf("Connection::recv_buf, recv_len: %d\n", recv_len);
    //     if  (recv_len <= 0)
    //         return -1;

    //     buf.add_exist(recv_len);
    //     printf("Connection::recv_buf, f: %llu, p: %llu\n", buf.f, buf.p);
    //     return recv_len;
    // }


    // int send_buf()
    // {
    //     int to_send = buf.get_exist_size();
    //     if  (to_send <= 0)
    //         return 0;
        
    //     struct iovec iov[2];
    //     size_t iov_len = 0;
    //     buf.get_exist_iovec(iov, &iov_len);

    //     struct msghdr msg = {
    //         .msg_name = NULL,
    //         .msg_namelen = 0,
    //         .msg_iov = iov,
    //         .msg_iovlen = iov_len,
    //         .msg_control = NULL,
    //         .msg_controllen = 0
    //     };
    //     int send_len = sendmsg(fd, &msg, 0);
    //     printf("Connection::send_buf, send_len: %d\n", send_len);
                
    //     if  (send_len < 0)
    //     {
    //         if  (errno == EAGAIN || errno == EWOULDBLOCK)
    //         {   send_len = 0;
    //         }

    //         else
    //             return -1;
    //     }

    //     buf.add_blank(send_len);


    //     printf("Connection::send_buf, f: %llu, p: %llu\n", buf.f, buf.p);
    //     return send_len;
    // }
    // int handle_read()
    // {
    //     if  (recv_buf() < 0)
    //         return -1;
    //     // if  (recv_buf_native() < 0)
    //     //     return -1;

    //     if  (send_buf() < 0)
    //         return -1;
    //     // if  (send_buf_native() < 0)
    //     //     return -1;
        
    //     return 0;
    // }

    // int handle_write()
    // {
    //     if  (send_buf() < 0)
    //         return -1;

    //     // if  (send_buf_native() < 0)
    //     //     return -1;
        
    //     return 0;
    // }


    int await_recv(char* buf, int size)
    {
        int ret = -1;


        while (true)
        {
            int recv_size = recv(fd, buf, size, 0);

            if  (recv_size <= 0)
            {

                if  (errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    wait_status = WaitStatus::wait_could_recv;

                    swapcontext(&this->ctx_fnew, &this->ctx_main);
                
                    wait_status = WaitStatus::running;
                }
                else
                {
                    ret = recv_size;
                    break;
                }
            }
            else
            {
                ret = recv_size;
                break;
            }
        }

        return ret;
    }

    int await_send(char* buf, int size)
    {
        int ret = 0;
        int have_sent = 0;


        while (have_sent < size)
        {
            char* start = buf + have_sent;
            int to_send = size - have_sent;

            int send_size = send(fd, start, to_send, 0);

            if  (send_size < 0)
            {
                if  (errno == EAGAIN || errno == EWOULDBLOCK)
                {

                    wait_status = WaitStatus::wait_could_send;

                    swapcontext(&this->ctx_fnew, &this->ctx_main);
                
                    wait_status = WaitStatus::running;
                }
                else
                {
                    ret = send_size;
                }
            }
            else
            {
                have_sent += send_size;
            }
        }

        return ret;
    }





    static void run(Connection* This)
    {
        printf("Connection::run: 1\n");


        while (true)
        {
            printf("Connection::run: 2\n");
            int recv_size = This->await_recv(This->buffer, This->buf_size);
            if  (recv_size <= 0)
                break;


            printf("Connection::run: 3\n");
            int send_size = This->await_send(This->buffer, recv_size);
            if  (send_size < 0)
                break;
        }


        printf("Connection::run: 4\n");
        This->exited = true;
        swapcontext(&This->ctx_fnew, &This->ctx_main);
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
            if  (wait_status == WaitStatus::wait_could_send)
            {
                swapcontext(&this->ctx_main, &this->ctx_fnew);
            }
        }
            
        if  (ev & EPOLLIN)
        {
            if  (wait_status == WaitStatus::wait_could_recv)
            {
                swapcontext(&this->ctx_main, &this->ctx_fnew);
            }
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

        conn->start();


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