#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>

const int buf_size = 4096;
// const short port = 8192;



struct sockaddr_storage ;


int display_local_address_by_hdr(struct msghdr* hdr)
{

    return 0;

}






int read_packet(int fd, char* buf, int size,
        struct sockaddr_storage* local_addr,
        struct sockaddr_storage* remote_addr)
{
    const int cmsg_buf_len = 128;
    char cbuf[cmsg_buf_len];

    struct iovec iov = {buf, (size_t)size};
    // struct sockaddr_storage raw_address;
    struct msghdr hdr;

    // hdr.msg_name = &raw_address;
    hdr.msg_name = remote_addr;
    hdr.msg_namelen = sizeof(sockaddr_storage);
    hdr.msg_iov = &iov;
    hdr.msg_iovlen = 1;
    hdr.msg_flags = 0;

    struct cmsghdr *cmsg = reinterpret_cast<struct cmsghdr *>(cbuf);
    cmsg->cmsg_len = sizeof(cbuf);
    hdr.msg_control = cmsg;
    hdr.msg_controllen = sizeof(cbuf);

    int bytes_read = recvmsg(fd, &hdr, 0);
    printf("recvmsg return %d\n", bytes_read);

    
    if (bytes_read < 0 && errno != 0)
    {
        if (errno != EAGAIN)
        {
            // LOG(ERROR) << "Error reading " << strerror(errno);
            perror("Error reading");
        }
        return -1;
    }

    if (hdr.msg_flags & MSG_CTRUNC)
    {
        // QUIC_BUG << "Incorrectly set control length: " << hdr.msg_controllen
        //          << ", expected " << QUIC_ARRAYSIZE(cbuf);
        
        printf("Incorrectly set control length: %d , expected: %d\n", sizeof(cbuf));

        return -1;
    }

    if  (display_local_address_by_hdr(&hdr) < 0)
    {
        printf("display local address error");
        return -1;
    }




    return bytes_read;
}

void* udp_echo(void* arg)
{
    int fd = *(int*)arg;
    char buf[buf_size];

    struct sockaddr_in client_addr;
    socklen_t socklen = sizeof(struct sockaddr_in);

    while (true)
    {
        struct sockaddr_storage local_addr, remote_addr;

        
        // int recv_len = recvfrom(fd, buf, buf_size, 0, (struct sockaddr*)&client_addr, &socklen);
        // if  (recv_len <= 0)  break;
        int recv_len = read_packet(fd, buf, buf_size, &local_addr, &remote_addr);
        if  (recv_len <= 0)  break;
        printf("server recv %d bytes \n", recv_len);

        if  (remote_addr.ss_family != AF_INET)   
        {
            printf("remote addr is not AF_INET, expect: %d, actual: %d, \n", AF_INET, remote_addr.ss_family);
            break;
        }
        struct sockaddr_in* remote_addr_in =(struct sockaddr_in*)&remote_addr;
        printf("server recv %d bytes from %s:%d\n", 
                recv_len, inet_ntoa(remote_addr_in->sin_addr), ntohs(remote_addr_in->sin_port));
        

        
        int send_len = sendto(fd, buf, recv_len, 0, (struct sockaddr*)&remote_addr, sizeof(struct sockaddr)); 
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
    {   printf("usage: %s <port>\n", argv[0]);
        return 0;
    }

    int port_;
    if  (sscanf(argv[1], "%d", &port_) == -1)  perror("bad port");
    // short port = port_;

    int fd = -1;
    if  ((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)  perror("socket error");

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons((short)port_);

    if  (bind(fd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) == -1)  perror("bind error");

    udp_echo(&fd);


    return 0;
}