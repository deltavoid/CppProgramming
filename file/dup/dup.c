#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>



#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <fcntl.h>



int fd1, fd2;


void* run(void* arg)
{
    int ret;
    struct sockaddr_in addr;
    int sin_size = sizeof(addr);


    printf("run: 1\n");
    ret = accept(fd2, (struct sockaddr*)&addr, (socklen_t*)&sin_size);

    printf("run: 2\n");
    return NULL;
}



int main()
{
    int ret;
    struct sockaddr_in addr;


    printf("main: 1\n");
    fd1 = socket(AF_INET, SOCK_STREAM, 0);
    if  (fd1 < 0)
    {   perror("socket error"); 
        return 1;
    }

    printf("main: 2\n");
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);
    addr.sin_addr.s_addr = INADDR_ANY;
    bzero(addr.sin_zero, sizeof(addr.sin_zero));

    printf("main: 3\n");
    ret = bind(fd1, (struct sockaddr*)&addr, sizeof(struct sockaddr));
    if  (ret < 0)
    {   perror("bind error");
        return 1;
    }

    printf("main: 4\n");
    ret = listen(fd1, 1000);
    if  (ret < 0)
    {   perror("listen error");
        return 1;
    }


    printf("main: 5\n");
    fd2 = dup(fd1);
    if  (fd2 < 0)
    {   perror("dup error");
        return 1;
    }


    printf("main: 6\n");
    close(fd1);


    printf("main: 7\n");
    pthread_t thread;
    ret = pthread_create(&thread, NULL, run, NULL);
    if  (ret < 0)
    {   perror("pthread_create error");
        return 1;
    }
    
    // printf("main: 7\n");
    // sleep(1);





    printf("main: 9\n");
    ret = pthread_join(thread, NULL);
    if  (ret < 0)
    {   perror("pthread_join error");
        return 1;
    }

    printf("main: 10\n");
    return 0;
}