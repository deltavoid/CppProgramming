#include <cstdio>
#include <cstdlib>




//此宏展开后，类似于printf("%d""%d", 1, 2);  
#define TRACE_CMH_2(fmt, args...) \  
    printf("%s(%d)-<%s>: " fmt, __FILE__, __LINE__, __FUNCTION__, ##args)  


int main()
{
    TRACE_CMH_2("hello\n");
}