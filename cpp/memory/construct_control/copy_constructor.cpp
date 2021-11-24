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

    String(const String& str)
    {
        printf("String::copy constructor begin\n");
        size = str.size;
        buf = new char[size];
        strcpy(buf, str.buf);
        buf[size] = '\0';

        printf("String::copy constructor end\n");        
    }

    String& operator = (const String& str)
    {
        printf("String::copy assign operator begin\n");
        
        if  (&str != this)
        {
            this->~String();

            new (this) String(str);
        }

        printf("String::copy assign operator end\n");
        return *this;
    }

    // String(String&& str)
    // {
    //     printf("String::move constructor begin\n");
    //     this->buf = str.buf;
    //     str.buf = NULL;

    //     this->size = str.size;
    //     str.size = 0;

    //     printf("String::move constructor end\n");
    // }


    void print()
    {
        if  (buf)
            printf("content: %s, size: %d\n", buf, size);
        else
            printf("string is NULL, size: %d\n", size);
    }
};


int main(int argc, char** argv)
{

    printf("1\n");
    {
        String str("hello");
        str.print();
        printf("\n");


        String str1(str);
        printf("\n");


        String str2 = str1;
        printf("\n");


        String str3("world");
        str3 = str2;
        printf("\n");


        str.print();
        str1.print();
        str2.print();
        str3.print();
        printf("\n");
        
    }
    printf("\n");


    printf("3\n");
    {
        String* str = new String("hello");
        str->print();
        printf("\n");


        // String str1(std::move(*str));
        String* str1 = new String(std::move(*str));
        printf("\n");
 

        str1->print();
        str->print();
        printf("\n");


        delete str;
        delete str1;
    }
    printf("\n");

    return 0;
}