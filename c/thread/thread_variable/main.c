#include <stdio.h>
#include <pthread.h>

// 线程局部变量，每个线程有独立的实例
_Thread_local int tls_counter = 0;

void* thread_func(void* arg) {
    int thread_num = *(int*)arg;
    for (int i = 0; i < 3; ++i) {
        tls_counter++; // 每个线程独立递增
        printf("Thread %d: tls_counter = %d\n", thread_num, tls_counter);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
    int id1 = 1, id2 = 2;

    pthread_create(&t1, NULL, thread_func, &id1);
    pthread_create(&t2, NULL, thread_func, &id2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}