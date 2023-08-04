#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <unistd.h>
#include <pthread.h>

#include <fcntl.h>
#include <sys/ioctl.h>

#include "../char_device_mmap_module/char_device_mmap.h"


// #define EXAMPLE_CHAR_DEVICE_PATH "/dev/example_mmap_char_device"




// void* run(void* arg)
// {
//     printf("run: 1\n");

//     sleep(10000);

//     return NULL;
// }


int mmap_char_device_test()
{
    int ret;
    printf("mmap_char_device_test\n");


    int dev_fd = open(EXAMPLE_CHAR_DEVICE_PATH, O_RDWR);
    if  (dev_fd < 0)
    {
        perror("dev open failed ");
    }

    int flags = fcntl(dev_fd, F_GETFL);
    printf("dev flags: %x\n", flags);


    int test_ioctl_arg = 1;
    ret = ioctl(dev_fd, /*test arg*/ 1, &test_ioctl_arg);
    printf("ioctl ret %d\n", ret);



    









    close(dev_fd);

}


int main()
{
    printf("main: 1\n");


    mmap_char_device_test();


    return 0;
}