#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>



int count;

struct context {

    int cnt;

};

static __thread struct context* local_context = NULL;


int context_init(struct context* ctx)
{
    ctx->cnt = 0;
    printf("count: %d\n", ++count);
}

void context_exit(struct context* ctx)
{
    ctx->cnt = 0;
    printf("count: %d\n", --count);
}

struct context* context_get()
{
    if  (local_context == NULL)
    {
        local_context = (struct context*)malloc(sizeof(struct context));

        context_init(local_context);
    }

    return local_context;
}





void* worker(void* arg)
{

    struct context* ctx = context_get();

    printf("ctx is %lx, count is %d\n", ctx, count);

    sleep(1);

    return NULL;
}


int main()
{



    for (int i = 0; i < 5; i++)
    {
        pthread_t thread;

        int ret = pthread_create(&thread, NULL, worker, NULL);

        int ret1 = pthread_join(thread, NULL);

        // int ret2 = pthread_exit(&thread);

        printf("\n");
    }

    return 0;
}