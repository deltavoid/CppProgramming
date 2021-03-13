#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>

#include <thread>
#include <memory>


class String {
public:

    char* buf;
    int size;

    String(const char* str)
    {
        printf("String::constructor begin\n");
        size = strlen(str);
        buf = new char[size + 1];
        strcpy(buf, str);
        buf[size] = '\0';

        printf("String::constructor end\n");
    }

    ~String()
    {
        printf("String::destructor begin\n");
        if  (buf)
            delete[] buf;

        printf("String::destructor end\n");
    }

    // String(String& str)
    // {

    // }

    String(String&& str)
    {
        printf("String::move constructor begin\n");
        this->buf = str.buf;
        str.buf = NULL;

        this->size = str.size;
        str.size = 0;

        printf("String::move constructor end\n");
    }

    void print()
    {
        if  (buf)
            printf("%s\n", buf);
        else
            printf("string is NULL\n");
    }
};


int main(int argc, char** argv)
{
    // printf("arg:\n");
    // for (int i = 0; i < argc; i++)
    //     printf("%s\n", argv[i]);


    String str("hello");
    str.print();

    String str1(std::move(str));

    str1.print();
    str.print();

    return 0;
}