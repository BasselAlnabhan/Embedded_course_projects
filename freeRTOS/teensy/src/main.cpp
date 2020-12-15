#include <Arduino.h>
#include <FreeRTOS_ARM.h>

#define DELAY (500U)

static SemaphoreHandle_t semaphore; // Declare a semaphore handle.

// The thread function for printing Ping.
static void print_ping(void *pvParameters)
{
    (void)(pvParameters);

    while (true)
    {
        // Sleep for 500 milliseconds.
        vTaskDelay(DELAY / portTICK_PERIOD_MS);

        Serial.print("Ping - ");
        Serial.flush();

        // Signal print_pong to print Pong.
        xSemaphoreGive(semaphore);
    }
}

// The thread function for printing Pong.
static void print_pong(void *pvParameters)
{
    (void)(pvParameters);

    while (true)
    {
        // Wait for signal from print_pong.
        xSemaphoreTake(semaphore, portMAX_DELAY);

        Serial.println("Pong");
        Serial.flush();
    }
}

// The thread function for blinking the LED every exact 5 seconds.
// Instead of vTaskDelay, we have used vTaskDelayUntil.
static void blinking(void *pvParameters)
{
    pinMode(LED_BUILTIN, OUTPUT);
    TickType_t xLastWakeTime = xTaskGetTickCount(); // Initialise the xLastWakeTime variable with the current time.

    while (true)
    {
        // Wait for the next cycle.
        vTaskDelayUntil(&xLastWakeTime, 10 * DELAY / portTICK_PERIOD_MS);

        // Turn LED on.
        digitalWrite(LED_BUILTIN, HIGH);

        // Sleep for 500 milliseconds.
        vTaskDelay(DELAY / portTICK_PERIOD_MS);

        // Turn LED off.
        digitalWrite(LED_BUILTIN, LOW);
    }
}

void setup()
{
    Serial.begin(9600);
    while (!Serial)
    {
        ;
    }

    // Initialize a binary semaphore
    semaphore = xSemaphoreCreateCounting(1, 0);

    // Create the task at priority one for print_ping
    BaseType_t ping = xTaskCreate(print_ping, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    // Create the task at priority two for print_pong
    BaseType_t pong = xTaskCreate(print_pong, NULL, configMINIMAL_STACK_SIZE, NULL, 2, NULL);

    // Create the task at priority three for blinking
    BaseType_t blink = xTaskCreate(blinking, NULL, configMINIMAL_STACK_SIZE, NULL, 3, NULL);

    // Check for creation errors
    if (semaphore == NULL || ping != pdPASS || pong != pdPASS || blink != pdPASS)
    {
        Serial.println("Creation error!");
        while (true)
        {
            ;
        }
    }

    // Start the scheduler
    vTaskStartScheduler();

    Serial.println("Failed to start the scheduler!");
    while (true)
    {
        ;
    }
}

void loop()
{
    // Not used.
}
