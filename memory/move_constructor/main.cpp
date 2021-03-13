#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>




class String {
public:

    char* buf;
    int size;

    String(const char* str)
    {
        size = strlen(str);
        buf = new char[size + 1];
        strcpy(buf, str);
        buf[size] = '\0';
    }

    ~String()
    {
        if  (buf)
            delete[] buf;
    }

    void print()
    {
        printf("%s\n", buf);
    }
};


int main(int argc, char** argv)
{
    // printf("arg:\n");
    // for (int i = 0; i < argc; i++)
    //     printf("%s\n", argv[i]);


    String str("hello");
    str.print();

    return 0;
}