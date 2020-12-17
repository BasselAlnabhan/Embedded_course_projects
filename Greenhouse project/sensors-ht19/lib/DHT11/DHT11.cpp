#include <DHT11.h>
#include <Metro.h>
#include <Arduino.h>

#define DATA_SIZE (5U)
#define BYTE_BITS (8U)
#define DATA_BITS (DATA_SIZE * BYTE_BITS)
#define CYCLES_BITS (2U * DATA_BITS)

#define TIMEOUT (0xFFFFFFFFU)
#define DHT11_INTERVAL (1500)

#define PREPARE (0U)
#define REQUEST (1U)
#define RESPONSE (2U)

static bool read(void);
static uint32_t expect_pulse(uint8_t level);

static uint32_t maxcycles = 0;
static uint8_t data[DATA_SIZE] = {};
static Metro metro = Metro(DHT11_INTERVAL);

void dht11_init(void)
{
	pinMode(DHT11_PIN, INPUT_PULLUP);
	maxcycles = microsecondsToClockCycles(1000);
}

uint8_t dht11_read(float *temperature, uint8_t *humidity)
{
	*humidity = 0;
	*temperature = NAN;
	static bool error = true;

	if (read())
	{
		*humidity = data[0];
		*temperature = data[2] + (data[3] & 0x0FU) * 0.1f;
		if (data[2] & 0x80U)
		{
			*temperature *= -1.0f;
		}

		error = false;
	}
	else if (metro.check())
	{
		if (error == true)
		{
			*temperature = 0.0f;
			return DHT11_ERROR;
		}
		else
		{
			error = true;
		}
	}

	uint8_t status = DHT11_OKAY;

	if (isnan(*temperature))
	{
		*temperature = 0.0f;
		status = DHT11_WAIT;
	}

	return status;
}

static bool read(void)
{
	static uint8_t state = PREPARE;

	if (state == PREPARE)
	{
		state = REQUEST;
		pinMode(DHT11_PIN, INPUT_PULLUP);
	}
	else if (state == REQUEST)
	{
		state = RESPONSE;
		pinMode(DHT11_PIN, OUTPUT);
		digitalWrite(DHT11_PIN, LOW);
	}
	else
	{
		state = PREPARE;
		uint32_t cycles[CYCLES_BITS] = {};

		pinMode(DHT11_PIN, INPUT_PULLUP);
		delayMicroseconds(60);

		if (expect_pulse(LOW) == TIMEOUT)
		{
			return false;
		}

		if (expect_pulse(HIGH) == TIMEOUT)
		{
			return false;
		}

		for (uint8_t i = 0; i < CYCLES_BITS; i += 2U)
		{
			cycles[i] = expect_pulse(LOW);
			cycles[i + 1] = expect_pulse(HIGH);
		}

		memset(data, 0, sizeof(data));

		for (uint8_t i = 0; i < DATA_BITS; ++i)
		{
			uint32_t low_cycles = cycles[2 * i];
			uint32_t high_cycles = cycles[2 * i + 1];
			if ((low_cycles == TIMEOUT) || (high_cycles == TIMEOUT))
			{
				return false;
			}

			data[i / BYTE_BITS] <<= 1;

			if (high_cycles > low_cycles)
			{
				data[i / BYTE_BITS] |= 1;
			}
		}

		return (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF));
	}

	return false;
}

static uint32_t expect_pulse(uint8_t level)
{
	uint32_t count = 0;

	while (digitalRead(DHT11_PIN) == level)
	{
		if (count++ >= maxcycles)
		{
			return TIMEOUT;
		}
	}

	return count;
}