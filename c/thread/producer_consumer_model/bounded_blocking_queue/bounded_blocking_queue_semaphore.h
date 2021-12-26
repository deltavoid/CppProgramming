#ifndef BOUNDED_BLOCKING_QUEUE_SEMAPHORE_H
#define BOUNDED_BLOCKING_QUEUE_SEMAPHORE_H

#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>



#define BOUNDED_BLOCKING_QUEUE_SEAMPHORE_DEFAULT_SIZE 128


struct bounded_blocking_queue_semaphore {
    void* que[BOUNDED_BLOCKING_QUEUE_SEAMPHORE_DEFAULT_SIZE];
    int f, p;

    pthread_mutex_t mutex;
    sem_t not_empty, not_full;

};


extern int bbqs_init(struct bounded_blocking_queue_semaphore* bbqs);
extern void bbqs_exit(struct bounded_blocking_queue_semaphore* bbqs);

extern void* bbqs_take(struct bounded_blocking_queue_semaphore* bbqs);
extern int bbqs_put(struct bounded_blocking_queue_semaphore* bbqs, void* item);

extern bool bbqs_empty(struct bounded_blocking_queue_semaphore* bbqs);



#endif // BOUNDED_BLOCKING_QUEUE_SEMAPHORE_H
