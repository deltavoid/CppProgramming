#ifndef HELLO_H
#define HELLO_H



class Hello {
public:
    virtual ~Hello() {}
    virtual void setId(int id) = 0;
    virtual int getId() = 0;
    virtual void printId() = 0;
};



#endif // HELLO_H
