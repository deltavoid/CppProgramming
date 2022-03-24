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


class CoroutineContext {
public:

    ucontext_t ctx_main, ctx_fnew;

    static const int stack_size = 4096;
    char stack[stack_size];

    bool exited;

    CoroutineContext() : exited(false) {}

    ~CoroutineContext() {}

    void init_context()
    {
        int ret = getcontext(&this->ctx_fnew);
        this->ctx_fnew.uc_stack.ss_sp = &this->stack;
	    this->ctx_fnew.uc_stack.ss_size = this->stack_size;
        this->ctx_fnew.uc_link = &this->ctx_main;
        // this->exited = false;
    }

};

class CoroutineSemaphore {
public:

    CoroutineContext* waiting_context;
    bool waiting;


    CoroutineSemaphore() : waiting_context(NULL), waiting(false) {}
    
    ~CoroutineSemaphore() {}

    int wait(CoroutineContext* ctx)
    {
        this->waiting_context = ctx;
        this->waiting = true;

        swapcontext(&ctx->ctx_fnew, &ctx->ctx_main);

        return 0;
    }

    int post()
    {
        CoroutineContext* ctx = this->waiting_context;
        this->waiting_context = NULL;
        this->waiting = false;

        int ret = swapcontext(&ctx->ctx_main, &ctx->ctx_fnew);

        return ret;
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

    CoroutineContext coroutine_context;

    CoroutineSemaphore recv_sem, send_sem;


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
        printf("Connection::~Connection\n");

        if  (epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL) < 0)
            perror("epoll_ctl del error");
        
        close(fd);
        printf("fd %d closed\n", fd);

        printf("Connection::~Connection: end\n");
    }


    int start()
    {
        int ret = -1;

        printf("Connection::start: 1\n");
        this->coroutine_context.init_context();

        printf("Connection::start: 2\n");
        makecontext(&this->coroutine_context.ctx_fnew, (void (*)(void)) run, 1, this);


        printf("Connection::start: 3\n");
        ret = swapcontext(&this->coroutine_context.ctx_main, &this->coroutine_context.ctx_fnew);
        if  (ret != 0)
        {   perror("swapcontext error at Connection::Connection");
        }


        printf("Connection::start: 4, end\n");
        return 0;
    }


    
    int await_recv(CoroutineContext* ctx, char* buf, int size)
    {
        int ret = -1;

        printf("connection::await_recv: 1\n");

        while (true)
        {
            int recv_size = recv(fd, buf, size, 0);

            if  (recv_size < 0)
            {
                if  (errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    printf("connection::await_recv: 2\n");

                    this->recv_sem.wait(ctx);
                    
                    printf("connection::await_recv: 3\n");
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

        printf("connection::await_recv: 4, ret: %d\n", ret);
        return ret;
    }

    int await_send(CoroutineContext* ctx, char* buf, int size)
    {
        int ret = 0;
        int have_sent = 0;

        printf("connection::await_send: 1\n");

        while (have_sent < size)
        {
            char* start = buf + have_sent;
            int to_send = size - have_sent;

            int send_size = send(fd, start, to_send, 0);

            if  (send_size < 0)
            {
                if  (errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    printf("connection::await_send: 2\n");
               
                    this->send_sem.wait(ctx);
            
                    printf("connection::await_send: 3\n");
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

        printf("connection::await_send: 4, ret: %d\n", ret);
        return ret;
    }





    static void run(Connection* This)
    {
        printf("Connection::run: 1\n");

        CoroutineContext* ctx = &This->coroutine_context;
        

        while (true)
        {
            printf("Connection::run: 2\n");
            int recv_size = This->await_recv(ctx, This->buffer, This->buf_size);
            if  (recv_size <= 0)
                break;


            printf("Connection::run: 3\n");
            int send_size = This->await_send(ctx, This->buffer, recv_size);
            if  (send_size < 0)
                break;
        }


        printf("Connection::run: 4\n");
        ctx->exited = true;
        swapcontext(&ctx->ctx_fnew, &ctx->ctx_main);
    }



    virtual int handle(uint32_t ev)
    {
        printf("Connection::handle: ev: %d\n", ev);

        if  (ev & ~(EPOLLIN | EPOLLOUT))
            return -1;

        if  (ev & EPOLLOUT)
        {
            if  (this->send_sem.waiting)
            {
                int ret = this->send_sem.post();

                if  (ret < 0 || this->coroutine_context.exited)
                    return -1;
            }
        }
            
        if  (ev & EPOLLIN)
        {
            if  (this->recv_sem.waiting)
            {
                int ret = this->recv_sem.post();
                
                if  (ret < 0 || this->coroutine_context.exited)
                    return -1;
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
