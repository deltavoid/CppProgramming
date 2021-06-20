#include "util.h"
#define MAXLINE 4096
#define SA struct sockaddr
 
//补齐的辅助数据块
union ctrl_t
{
    struct cmsghdr cmsg;
    char buff[CMSG_SPACE(sizeof(struct in_pktinfo))];
}ctrl_un;
 
int main(int argc, char ** argv)
{
    //udp socket
    int sock = socket(AF_INET, SOCK_DGRAM,0);
    struct sockaddr_in serv_addr,cli_addr; 
    memset(&serv_addr,0,sizeof(serv_addr));
    memset(&cli_addr,0,sizeof(cli_addr));
    serv_addr.sin_family= AF_INET;
    serv_addr.sin_port = htons(8090);
    serv_addr.sin_addr.s_addr = 0;
    if(bind(sock,(SA*)&serv_addr,sizeof(serv_addr)) < 0){
        perror("bind");
        return -1;
    }
 
    const int on = 1;
 
    //开启获取包信息 , 结果存放在辅助数据当中
    setsockopt(sock,IPPROTO_IP,IP_PKTINFO,&on,sizeof(on));
 
    //接受缓冲区
    char recvbuf[MAXLINE] = {0};
 
    //用于一次性写入/读取多个缓冲区,详见writev/readv;
    struct iovec iov[1];
 
    //定义一个 recvmsg 需要的结构
    struct msghdr msg = {0};
 
    //指定辅助数据的指针
    msg.msg_control = ctrl_un.buff;
    
    //flags见man
    msg.msg_flags = 0;
 
    //指定缓冲区
    iov[0].iov_base = recvbuf;
    iov[0].iov_len = MAXLINE;
    msg.msg_iov = iov;
 
    //仅适用于udp socket. 用于接受客户的ip , port. 类似recvfrom 最后2个参数
    msg.msg_name = &cli_addr;
 
    //recvmsg返回值
    int retvalue = 0;
 
    //用于获取辅助数据
    struct cmsghdr *pcmsg = NULL;
 
    //2个存放ip地址的缓冲区,可用可不用
    char route_ip_buf[16];
    char dst_ip_buf[16];
 
 
    while(1){
        
        //由于这些长度变量都是值-结果,因此每次重置
        msg.msg_iovlen = 1;
        msg.msg_controllen = sizeof(ctrl_un.buff);
        msg.msg_namelen = sizeof(cli_addr);
 
        //类似于recvfrom
        if ((retvalue=recvmsg(sock,&msg,0)) < 0){
            perror("recvmsg error:");
            break;
        }
 
        
        //开始获取辅助数据,由于辅助数据可以是一个也可以是一个数组,因此循环;
        for(pcmsg = CMSG_FIRSTHDR(&msg) ; pcmsg != NULL ; pcmsg = CMSG_NXTHDR(&msg,pcmsg))
        {
 
            //判断是否是包信息
            if(pcmsg->cmsg_level == IPPROTO_IP &&
                    pcmsg->cmsg_type == IP_PKTINFO){
 
                //获取我们的自定义数据 struct in_pktinfo ;
                unsigned char * pData = CMSG_DATA(pcmsg);
                struct in_pktinfo * pInfo = (struct in_pktinfo *)pData;
 
                //转换
                inet_ntop(AF_INET,&pInfo->ipi_addr,dst_ip_buf,sizeof(dst_ip_buf));
                inet_ntop(AF_INET,&pInfo->ipi_spec_dst,route_ip_buf,sizeof(route_ip_buf));
 
                //下面都是打印信息
                printf("client_addr:%s,port:%d\n",inet_ntoa(cli_addr.sin_addr),
                ntohs(cli_addr.sin_port));
                printf("route ip :%s, dst ip:%s , ifindex:%d\n" , route_ip_buf,dst_ip_buf,
                pInfo->ipi_ifindex);
                recvbuf[retvalue] = 0;
                printf("recv bytes:%d , recvbuf:%s \n", retvalue, recvbuf);
            }
        }
 
    }
 
 
 
    return 0;
}