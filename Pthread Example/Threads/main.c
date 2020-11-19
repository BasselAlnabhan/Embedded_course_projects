/**
 * Create a program with two threads where one thread writes "Ping" and the other thread writes "Pong".
 * Ensure the right order so the printout will look like:
 * Ping - Pong
 * Ping - Pong
 * Ping - Pong
 * ...
 * The application shall do this 10 times before exit. 
**/

#include <stdio.h>
#include <stdint.h>
#include <pthread.h>

#define PING 0
#define PONG 1
#define ITRATIONS 10

static void *print_ping(void *); // The ping thread function
static void *print_pong(void *); // The pong thread function

static uint8_t ping_pong = PING;                            // The shared variable
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;   // The mutex to control access to the shared variable
static pthread_cond_t condition = PTHREAD_COND_INITIALIZER; // The condition variable used to synchronize the threads

int main(void)
{
    pthread_t ping_thread, pong_thread;

    // Create and check the ping thread
    if (pthread_create(&ping_thread, NULL, print_ping, NULL))
    {
        printf("Faild to create ping_thread!\n");
        return 1;
    }

    // Create and check the pong thread
    if (pthread_create(&pong_thread, NULL, print_pong, NULL))
    {
        printf("Faild to create pong_thread!\n");
        return 1;
    }

    // Wait on the threads get finished
    pthread_join(ping_thread, NULL);
    pthread_join(pong_thread, NULL);

    // Release the resources
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condition);

    return 0;
}

static void *print_ping(void *arg)
{
    (void)arg;

    for (uint8_t i = 0; i < ITRATIONS; i++)
    {
        pthread_mutex_lock(&mutex); // Lock the mutex

        if (ping_pong != PING)
        {
            pthread_cond_wait(&condition, &mutex); // Wait until the contion variable gets signaled
        }

        printf("Ping - ");
        ping_pong = PONG;

        pthread_cond_signal(&condition); // Signal the waiting thread (pong thread).

        pthread_mutex_unlock(&mutex); // Release the mutex
    }

    pthread_exit(NULL); // Exit the thread without a return value
}

static void *print_pong(void *arg)
{
    (void)arg;

    for (uint8_t i = 0; i < ITRATIONS; i++)
    {
        pthread_mutex_lock(&mutex); // Lock the mutex

        if (ping_pong != PONG)
        {
            pthread_cond_wait(&condition, &mutex); // Wait until the contion variable gets signaled
        }

        printf("Pong\n");
        ping_pong = PING;

        pthread_cond_signal(&condition); // Signal the waiting thread (ping thread).

        pthread_mutex_unlock(&mutex); // Release the mutex
    }

    pthread_exit(NULL); // Exit the thread without a return value
}