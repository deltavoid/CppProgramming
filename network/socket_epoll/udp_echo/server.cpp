#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>


const int buf_size = 4096;
// const short port = 8192;



void* udp_echo(void* arg)
{
    int fd = *(int*)arg;
    char buf[buf_size];

    struct sockaddr_in client_addr;
    socklen_t socklen = sizeof(struct sockaddr_in);

    while (true)
    {
        int recv_len = recvfrom(fd, buf, buf_size, 0, (struct sockaddr*)&client_addr, &socklen);
        if  (recv_len <= 0)  break;
        printf("server recv %d bytes from %s:%d\n", 
                recv_len, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        
        int send_len = sendto(fd, buf, recv_len, 0, (struct sockaddr*)&client_addr, socklen); 
        printf("server echo %d bytes to %s:%d\n",
                send_len, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        printf("\n");
    }

    close(fd);
    printf("socket %d closed\n", fd);
    return NULL;
}




int main(int argc, char** argv)
{
    if  (!(argc > 1))
    {   printf("usage: %s <port> [<addr>]\n", argv[0]);
        return 0;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(sockaddr_in));
    server_addr.sin_family = AF_INET;
    
    int port_;
    if  (sscanf(argv[1], "%d", &port_) == -1)  perror("bad port");
    // short port = port_;
    server_addr.sin_port = htons((short)port_);    


    if  (argc > 2)
    {
        if  (inet_pton(AF_INET, argv[2], &server_addr.sin_addr.s_addr) < 0)
            perror("bad addr");
        
        char addr[20];
        printf("addr: %s\n", inet_ntop(AF_INET, &server_addr.sin_addr.s_addr, addr, 20));
    }
    else
    {
        server_addr.sin_addr.s_addr = INADDR_ANY;
    }

    int fd = -1;
    if  ((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)  perror("socket error");

    if  (bind(fd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) == -1)  perror("bind error");

    udp_echo(&fd);


    return 0;
}