#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <unistd.h>
#include <pthread.h>


#define EXAMPLE_CHAR_DEVICE_PATH "/dev/example_mmap_char_device"




// void* run(void* arg)
// {
//     printf("run: 1\n");

//     sleep(10000);

//     return NULL;
// }


void mmap_char_device_test()
{
    printf("mmap_char_device_test\n");

}


int main()
{
    printf("main: 1\n");

    // pthread_t thread;
    // {
    //     int ret = pthread_create(&thread, NULL, run, NULL);
    // }

    // printf("main: 2\n");

    // {
    //     int ret = pthread_join(thread, NULL);
    // }

    // printf("main: 3\n");


    // printf("hello world\n");
    mmap_char_device_test();

    // sleep(10000);


    return 0;
}