#ifndef CONFIG_H
#define CONFIG_H




struct Config {

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

};



#endif // CONFIG_H
