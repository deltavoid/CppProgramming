#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <pthread.h>



#define THREAD_NUM 4

pthread_t threads[THREAD_NUM];


int thread_ids[THREAD_NUM];





void* worker_run(void* arg)
{
    int id = *(int*)arg;
    int i;


    for (i = 0; i < 3; i++)
    {
        printf("worker: %d, cnt: %d\n", id, i);

        sleep(1);
    }


    return NULL;
}




int main(int argc, char** argv, char** env)
{
    // printf("env:\n");
    // for (int i = 0; env[i] != NULL; i++)
    //     printf("%s\n", env[i]);
    // printf("\n");

    // printf("arg:\n");
    // for (int i = 0; i < argc; i++)
    //     printf("%s\n", argv[i]);

    int ret;
    int i;

    for (i = 0; i < THREAD_NUM; i++)
    {
        thread_ids[i] = i;

        ret = pthread_create(&threads[i], NULL, worker_run, (void*)&thread_ids[i]);
        if  (ret < 0)
            return -1;
    }


    for (i = 0; i < THREAD_NUM; i++)
        pthread_join(threads[i], NULL);    

    return 0;
}