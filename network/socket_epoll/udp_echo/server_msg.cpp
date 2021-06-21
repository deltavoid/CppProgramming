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



int get_local_address_by_hdr(struct msghdr* hdr, struct sockaddr_storage* local_addr)
{
    if  (hdr->msg_controllen > 0)
    {
        for (cmsghdr *cmsg = CMSG_FIRSTHDR(hdr); cmsg != nullptr; cmsg = CMSG_NXTHDR(hdr, cmsg))
        {
            if (cmsg->cmsg_type == IP_PKTINFO)
            {
                in_pktinfo *info = reinterpret_cast<in_pktinfo *>(CMSG_DATA(cmsg));

                char route_ip_buf[128], dst_ip_buf[128];
                inet_ntop(AF_INET, &info->ipi_addr, dst_ip_buf, sizeof(dst_ip_buf));
                inet_ntop(AF_INET, &info->ipi_spec_dst, route_ip_buf, sizeof(route_ip_buf));

                printf("route ip: %s, dst ip: %s, ifindex: %d\n", 
                        route_ip_buf, dst_ip_buf, info->ipi_ifindex);

                struct sockaddr_in* local_addr_in = (struct sockaddr_in*)local_addr;
                local_addr_in->sin_family = AF_INET;
                local_addr_in->sin_addr = info->ipi_addr;
                local_addr_in->sin_port = 0;
                bzero(local_addr_in->sin_zero, sizeof(local_addr_in->sin_zero));
            }
        }
    }

    return 0;
}


int read_packet(int fd, char* buf, int size,
        struct sockaddr_storage* local_addr,
        struct sockaddr_storage* remote_addr)
{
    if  (local_addr == NULL || remote_addr == NULL)
        return -1;
    
    const int cmsg_buf_len = 128;
    char cbuf[cmsg_buf_len];

    struct iovec iov = {buf, (size_t)size};
    struct msghdr hdr;

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

    
    if  (bytes_read < 0 && errno != 0)
    {   if  (errno != EAGAIN)
        {   perror("Error reading");
        }
        return -1;
    }

    if (hdr.msg_flags & MSG_CTRUNC)
    {
        printf("Incorrectly set control length: %d , expected: %d\n", sizeof(cbuf));
        return -1;
    }

    if  (get_local_address_by_hdr(&hdr, local_addr) < 0)
    {
        printf("get local address error");
        return -1;
    }


    return bytes_read;
}

void* udp_echo(void* arg)
{
    int fd = *(int*)arg;
    char buf[buf_size];


    while (true)
    {
        struct sockaddr_storage local_addr, remote_addr;
        int recv_len = read_packet(fd, buf, buf_size, &local_addr, &remote_addr);
        if  (recv_len <= 0)  break;
        printf("server recv %d bytes \n", recv_len);

        if  (remote_addr.ss_family != AF_INET)   
        {   printf("remote addr is not AF_INET, expect: %d, actual: %d, \n", AF_INET, remote_addr.ss_family);
            break;
        }
        struct sockaddr_in* remote_addr_in =(struct sockaddr_in*)&remote_addr;
        printf("server recv %d bytes from %s:%d\n", 
                recv_len, inet_ntoa(remote_addr_in->sin_addr), ntohs(remote_addr_in->sin_port));

        if  (local_addr.ss_family != AF_INET)
        {   printf("local addr is not AF_INET\n");
            break;
        }   
        struct sockaddr_in* local_addr_in = (struct sockaddr_in*)&local_addr;
        printf("server recv %d bytes at %s\n", recv_len,
                inet_ntoa(local_addr_in->sin_addr));

 
        int send_len = sendto(fd, buf, recv_len, 0, (struct sockaddr*)&remote_addr, sizeof(struct sockaddr)); 
        printf("server echo %d bytes to %s:%d\n",
                send_len, inet_ntoa(remote_addr_in->sin_addr), ntohs(remote_addr_in->sin_port));
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
    if  ((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)  
    {   perror("socket error");
        return 1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons((short)port_);

    if  (bind(fd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) == -1)  
    {   perror("bind error");
        return 1;
    }

    const int on = 1;
    if  (setsockopt(fd, IPPROTO_IP, IP_PKTINFO, &on, sizeof(on)) < 0)
    {   perror("setsockopt");
        return 1;
    }


    udp_echo(&fd);


    return 0;
}