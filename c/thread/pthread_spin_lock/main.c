#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <pthread.h>





struct resource {
    int cnt;
    pthread_spinlock_t lock;
};

int resource_init(struct resource* obj)
{
    obj->cnt = 0;
    pthread_spin_init(&obj->lock, 0);
}

void resource_exit(struct resource* obj)
{
    pthread_spin_destroy(&obj->lock);
}

int resource_inc_cnt(struct resource* obj)
{
    int ret;

    pthread_spin_lock(&obj->lock);

    ret = ++obj->cnt;

    pthread_spin_unlock(&obj->lock);

    return ret;
}




struct worker {
    int id;
    struct resource* res;
};

int worker_init(struct worker* obj, int id, struct resource* res)
{
    obj->id = id;
    obj->res = res;
}

void* worker_run(void* arg)
{
    struct worker* worker = (struct worker*)arg;
    int i;


    for (i = 0; i < 3; i++)
    {
        int cnt = resource_inc_cnt(worker->res);

        printf("worker: %d, i: %d, res cnt: %d\n", worker->id, i, cnt);

        sleep(1);
    }


    return NULL;
}



#define THREAD_NUM 4

pthread_t threads[THREAD_NUM];

struct worker workers[THREAD_NUM];


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
    struct resource res;

    resource_init(&res);

    for (i = 0; i < THREAD_NUM; i++)
    {
        worker_init(&workers[i], i, &res);

        ret = pthread_create(&threads[i], NULL, worker_run, (void*)&workers[i]);
        if  (ret < 0)
            return -1;
    }


    for (i = 0; i < THREAD_NUM; i++)
        pthread_join(threads[i], NULL);    

    return 0;
}