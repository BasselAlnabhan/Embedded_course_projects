/**
 * @brief Multithreading using TeensyThreads. 
 * In this example, 3 threads and loop are synchornized in a way that the threads print Pin -, Pong and Done in order.
 * @date 2020-11-23
 */

#include <Arduino.h>
#include <TeensyThreads.h>

#define PING (0U)
#define PONG (1U)
#define DONE (2U)

static Threads::Mutex mutex;
static volatile uint8_t ppd = PING;

static void print_ping(void)
{
    while (true)
    {
        threads.delay(500); // This function makes a context switch

        mutex.lock(); // Acquires the mutex
        while (ppd != PING)
        {
            // These 3 lines of code are equivalent to wait in condition variables
            mutex.unlock();
            threads.yield(); // This function is used to suspend the task and pass the control to a waiting thread. A context switch occures.
            mutex.lock();
        }

        Serial.print("Ping - ");
        ppd = PONG;

        mutex.unlock();  // Releases the mutex
        threads.yield(); // A context switch occures.
    }
}

static void print_pong(void)
{
    while (true)
    {
        mutex.lock();
        while (ppd != PONG)
        {
            mutex.unlock();
            threads.yield();
            mutex.lock();
        }

        Serial.print("Pong : ");
        ppd = DONE;

        mutex.unlock();
        threads.yield();
    }
}

static void print_done(void)
{
    while (true)
    {
        mutex.lock();
        while (ppd != DONE)
        {
            mutex.unlock();
            threads.yield();
            mutex.lock();
        }

        Serial.println("Done");
        ppd = PING;

        mutex.unlock();
        threads.yield();
    }
}

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(9600);
    while (!Serial)
    {
    }

    // Creating the threads
    threads.addThread(print_ping);
    threads.addThread(print_pong);
    threads.addThread(print_done);
}

void loop()
{
    digitalWrite(LED_BUILTIN, LOW);
    threads.delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
    threads.delay(500);
}