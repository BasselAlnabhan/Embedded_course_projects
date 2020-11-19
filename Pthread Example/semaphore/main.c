#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define ITERATIONS (20)
#define BUFFER_SIZE (5)
#define MAX_ITEMS (5)

static sem_t empty, full;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static int buffer[BUFFER_SIZE] = {}, in = 0, out = 0;

void *producer(void *args)
{
    int item;
    (void)args;
    for (int i = 0; i < MAX_ITEMS; i++)
    {
        sem_wait(&empty);
        item = rand();
        if (out == in)
        {
            printf("Stock is full, producer waiting...");
        }
        else
        {
            buffer[in] = item;
            in = (in + 1) % BUFFER_SIZE;
        }
        sleep(1);
        sem_post(&full);
    }
}

void *consumer(void *args)
{
    (void)args;
    for (int i = 0; i < MAX_ITEMS; i++)
    {
        sem_wait(&full);
        if (out == in)
        {
            printf("Stock is empty, consumer waiting...");
        }
        else
        {
            printf("%d\n", buffer[out]);
            out = (out - 1) % BUFFER_SIZE;
        }
        sleep(2);
        sem_post(&full);
    }
}

void main(void)
{
    pthread_t pro, con;

    sem_init(&empty, 1, 1);
    sem_init(&full, 1, 0);

    pthread_create(&pro, NULL, producer, NULL);
    pthread_create(&con, NULL, consumer, NULL);

    pthread_join(pro, NULL);
    pthread_join(con, NULL);

    pthread_mutex_destroy(&mutex);
}