#include <cstdio>
#include <cstdlib>
#include <queue>
#include <random>
#include <pthread.h>
#include <unistd.h>

typedef long long ll;

const ll queue_size = 10;
std::queue<ll> queue;
pthread_mutex_t mutex;
pthread_cond_t not_empty;
pthread_cond_t not_full;

bool running = true;


void* consumer(void* arg)
{
    ll id = (ll)arg;
    printf("consumer: %lld\n", id);

    while (running)
    {
        pthread_mutex_lock(&mutex);
        while (queue.size() == 0)
            pthread_cond_wait(&not_empty, &mutex);
        
        ll item = queue.front();  
        queue.pop();
        printf("consumer %lld: %lld\n", id, item);

        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&not_full);
        // pthread_cond_broadcast(&not_full);

        sleep(1);
    }

    pthread_exit(NULL);
}

void* producer(void* arg)
{
    ll id = (ll)arg;
    printf("producer: %lld\n", id);

    std::mt19937 rnd(time(NULL) + id);

    while (running)
    {
        pthread_mutex_lock(&mutex);
        while (queue.size() == queue_size)
            pthread_cond_wait(&not_full, &mutex);
        
        ll item = rnd();
        queue.push(item);
        printf("producer %lld: %lld\n", id, item);

        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&not_empty);
        
        sleep(1);
    }

    pthread_exit(NULL);
}

void test1()
{
    pthread_t threads[2];
    {
        int ret = pthread_create(&threads[0], NULL, consumer, (void*)0);
    }
    {
        int ret = pthread_create(&threads[1], NULL, producer, (void*)1);
    }

    sleep(100);
    running = false;
    for (ll i = 0; i < 2; i++)
    {
        int ret = pthread_join(threads[i], NULL);
    }
}

void test2()
{
    const ll num_consumer = 4;
    const ll num_producer = 4;
    pthread_t consumer_threads[num_consumer];
    pthread_t producer_threads[num_producer];

    for (ll i = 0; i< num_consumer; i++)
    {
        int ret = pthread_create(&consumer_threads[i], NULL, consumer, (void*)i);
    }
    for (ll i = 0; i < num_producer; i++)
    {
        int ret = pthread_create(&producer_threads[i], NULL, producer, (void*)i);
    }

    // sleep(100);
    for (ll i = 0; i < 100; i++)
    {   printf("--------------------------\n");
        sleep(1);
    }
    running = false;
    for (ll i = 0; i < num_consumer; i++)
    {   
        int ret = pthread_join(consumer_threads[i], NULL);
    }
    for (ll i = 0; i < num_producer; i++)
    {
        int ret = pthread_join(producer_threads[i], NULL);
    }

}


int main()
{
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&not_empty, NULL);
    pthread_cond_init(&not_full, NULL);

    // test1();
    test2();

    pthread_cond_destroy(&not_empty);
    pthread_cond_destroy(&not_full);
    pthread_mutex_destroy(&mutex);

    return 0;
}