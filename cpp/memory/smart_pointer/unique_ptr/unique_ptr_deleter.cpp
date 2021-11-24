#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>

#include <thread>
#include <memory>



struct connection {
    int fd;
};


int connection_init(struct connection* conn)
{
    printf("connection_init: %lx\n", conn);
}

void connection_exit(struct connection* conn)
{
    printf("connnection_exit: %lx\n", conn);
}




int main(int argc, char** argv)
{

    struct connection* conn = (struct connection*)malloc(sizeof(struct connection));
    connection_init(conn);


    {
        std::unique_ptr<struct connection, decltype(connection_exit)* > conn_ptr(conn, 
            [](struct connection* conn) {
                connection_exit(conn);
                free(conn);
            }
        );


              
    }

    return 0;
}