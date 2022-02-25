#ifndef CONNECTION_FACTORY_H
#define CONNECTION_FACTORY_H



class ConnectionFactory {
public:

    virtual int make_connection(int fd, int epfd) {  return -1;}

};



#endif // CONNECTION_FACTORY_H
