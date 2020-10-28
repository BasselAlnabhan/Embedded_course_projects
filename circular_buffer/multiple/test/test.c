#include <unity.h>
#include <circular_buffer.h>

#define BUFSIZE (5U)

static circular_buffer_t *cbuffer = NULL;

void setUp()
{
    // Create the instance
    cbuffer = circular_buffer_create(BUFSIZE);
    TEST_ASSERT_NOT_NULL(cbuffer);
}

void tearDown()
{
    // Destroy the instance
    circular_buffer_destroy(cbuffer);
    cbuffer = NULL;
}

static void write_buffer(uint8_t min, uint8_t max)
{
    bool full = false;
    uint8_t value = 0U;

    // Fill (max - min) elements of the buffer with min to (max - 1)
    for (uint8_t i = min; i < max; i++)
    {
        TEST_ASSERT_TRUE(circular_buffer_write(cbuffer, i));          // Test if write function returns true
        TEST_ASSERT_TRUE(circular_buffer_available(cbuffer, &value)); // Test if available function returns true
        TEST_ASSERT_EQUAL_UINT8(i, value);                            // Test the number of available data
        TEST_ASSERT_TRUE(circular_buffer_isfull(cbuffer, &full));     // Test if isfull function returns true
        TEST_ASSERT_FALSE(full);                                      // Test if the buffer is not full
    }
}

void test_circular_buffer_create(void)
{
    bool full = false;
    uint8_t value = 0U;

    // Test if the buffer is not full
    TEST_ASSERT_TRUE(circular_buffer_isfull(cbuffer, &full));
    TEST_ASSERT_FALSE(full);

    // Test the buffer is empty
    TEST_ASSERT_TRUE(circular_buffer_available(cbuffer, &value));
    TEST_ASSERT_EQUAL_UINT8(0U, value);

    // Test the invalid buffer size
    TEST_ASSERT_NULL(circular_buffer_create(0));
}

void test_read_write_buffer(void)
{
    bool full = false;
    uint8_t value = 0U;

    write_buffer(1U, BUFSIZE); // Fill (BUFSIZE - 1) elements of the buffer with 1 to (BUFSIZE - 1)

    // Test the peek function
    TEST_ASSERT_TRUE(circular_buffer_peek(cbuffer, &value));
    TEST_ASSERT_EQUAL_UINT8(1U, value);
    TEST_ASSERT_TRUE(circular_buffer_available(cbuffer, &value));
    TEST_ASSERT_EQUAL_UINT8(BUFSIZE - 1U, value);
    TEST_ASSERT_TRUE(circular_buffer_isfull(cbuffer, &full));
    TEST_ASSERT_FALSE(full);

    // Test the read function
    TEST_ASSERT_TRUE(circular_buffer_read(cbuffer, &value));
    TEST_ASSERT_EQUAL_UINT8(1U, value);
    TEST_ASSERT_TRUE(circular_buffer_available(cbuffer, &value));
    TEST_ASSERT_EQUAL_UINT8(BUFSIZE - 2U, value);
    TEST_ASSERT_TRUE(circular_buffer_isfull(cbuffer, &full));
    TEST_ASSERT_FALSE(full);
}

void test_reset_buffer(void)
{
    bool full = false;
    uint8_t value = 0U;

    write_buffer(1U, BUFSIZE); // Fill (BUFSIZE - 1) elements of the buffer with 1 to (BUFSIZE - 1)

    // Test the reset function
    TEST_ASSERT_TRUE(circular_buffer_reset(cbuffer));
    TEST_ASSERT_TRUE(circular_buffer_available(cbuffer, &value));
    TEST_ASSERT_EQUAL_UINT8(0U, value);
    TEST_ASSERT_TRUE(circular_buffer_isfull(cbuffer, &full));
    TEST_ASSERT_FALSE(full);
}

void test_read_empty_buffer(void)
{
    bool full = false;
    uint8_t value = 0U;

    // Test reading from an empty buffer
    TEST_ASSERT_FALSE(circular_buffer_read(cbuffer, &value));
    TEST_ASSERT_TRUE(circular_buffer_available(cbuffer, &value));
    TEST_ASSERT_EQUAL_UINT8(0U, value);
    TEST_ASSERT_TRUE(circular_buffer_isfull(cbuffer, &full));
    TEST_ASSERT_FALSE(full);

    write_buffer(1U, BUFSIZE); // Fill (BUFSIZE - 1) elements of the buffer with 1 to (BUFSIZE - 1)

    // Fill the last element of the buffer with BUFSIZE and make the buffer full
    TEST_ASSERT_TRUE(circular_buffer_write(cbuffer, BUFSIZE));
    TEST_ASSERT_TRUE(circular_buffer_available(cbuffer, &value));
    TEST_ASSERT_EQUAL_UINT8(BUFSIZE, value);
    TEST_ASSERT_TRUE(circular_buffer_isfull(cbuffer, &full));
    TEST_ASSERT_TRUE(full);

    // Overwrite oldest element in the buffer with BUFSIZE + 1U
    TEST_ASSERT_TRUE(circular_buffer_write(cbuffer, BUFSIZE + 1U));
    TEST_ASSERT_TRUE(circular_buffer_available(cbuffer, &value));
    TEST_ASSERT_EQUAL_UINT8(BUFSIZE, value);
    TEST_ASSERT_TRUE(circular_buffer_isfull(cbuffer, &full));
    TEST_ASSERT_TRUE(full);

    // Read all the elements of the buffer and make it empty
    for (uint8_t i = 1U; i <= BUFSIZE; i++)
    {
        TEST_ASSERT_TRUE(circular_buffer_read(cbuffer, &value));
        TEST_ASSERT_EQUAL_UINT8(i + 1U, value);
        TEST_ASSERT_TRUE(circular_buffer_available(cbuffer, &value));
        TEST_ASSERT_EQUAL_UINT8(BUFSIZE - i, value);
        TEST_ASSERT_TRUE(circular_buffer_isfull(cbuffer, &full));
        TEST_ASSERT_FALSE(full);
    }

    TEST_ASSERT_FALSE(circular_buffer_read(cbuffer, &value));     // Test if reading from an empty buffer returns false
    TEST_ASSERT_TRUE(circular_buffer_available(cbuffer, &value)); // Test if available function returns true
    TEST_ASSERT_EQUAL_UINT8(0U, value);                           // Test if the buffer is empty
    TEST_ASSERT_TRUE(circular_buffer_isfull(cbuffer, &full));     // Test if the buffer is not full
    TEST_ASSERT_FALSE(full);
}

void test_read_full_buffer(void)
{
    bool full = false;
    uint8_t value = 0U;

    write_buffer(1U, BUFSIZE); // Fill (BUFSIZE - 1) elements of the buffer with 1 to (BUFSIZE - 1)

    // Fill the last element of the buffer with BUFSIZE and make the buffer full
    TEST_ASSERT_TRUE(circular_buffer_write(cbuffer, BUFSIZE));
    TEST_ASSERT_TRUE(circular_buffer_available(cbuffer, &value));
    TEST_ASSERT_EQUAL_UINT8(BUFSIZE, value);
    TEST_ASSERT_TRUE(circular_buffer_isfull(cbuffer, &full));
    TEST_ASSERT_TRUE(full);

    // Test peeking
    TEST_ASSERT_TRUE(circular_buffer_peek(cbuffer, &value));
    TEST_ASSERT_EQUAL_UINT8(1U, value);
    TEST_ASSERT_TRUE(circular_buffer_available(cbuffer, &value));
    TEST_ASSERT_EQUAL_UINT8(BUFSIZE, value);
    TEST_ASSERT_TRUE(circular_buffer_isfull(cbuffer, &full));
    TEST_ASSERT_TRUE(full);

    // Test reading from a full buffer
    TEST_ASSERT_TRUE(circular_buffer_read(cbuffer, &value));
    TEST_ASSERT_EQUAL_UINT8(1U, value);
    TEST_ASSERT_TRUE(circular_buffer_available(cbuffer, &value));
    TEST_ASSERT_EQUAL_UINT8(BUFSIZE - 1U, value);
    TEST_ASSERT_TRUE(circular_buffer_isfull(cbuffer, &full));
    TEST_ASSERT_FALSE(full);
}

void test_write_empty_buffer(void)
{
    bool full = false;
    uint8_t value = 0U;

    write_buffer(1U, BUFSIZE); // Fill (BUFSIZE - 1) elements of the buffer with 1 to (BUFSIZE - 1)

    // Fill the last element of the buffer with BUFSIZE and make the buffer full
    TEST_ASSERT_TRUE(circular_buffer_write(cbuffer, BUFSIZE));
    TEST_ASSERT_TRUE(circular_buffer_available(cbuffer, &value));
    TEST_ASSERT_EQUAL_UINT8(BUFSIZE, value);
    TEST_ASSERT_TRUE(circular_buffer_isfull(cbuffer, &full));
    TEST_ASSERT_TRUE(full);

    // Test reading all the elements of the buffer and make it empty
    for (uint8_t i = 1U; i <= BUFSIZE; i++)
    {
        TEST_ASSERT_TRUE(circular_buffer_read(cbuffer, &value));
        TEST_ASSERT_EQUAL_UINT8(i, value);
        TEST_ASSERT_TRUE(circular_buffer_available(cbuffer, &value));
        TEST_ASSERT_EQUAL_UINT8(BUFSIZE - i, value);
        TEST_ASSERT_TRUE(circular_buffer_isfull(cbuffer, &full));
        TEST_ASSERT_FALSE(full);
    }

    // Test reading from an empty buffer
    TEST_ASSERT_FALSE(circular_buffer_read(cbuffer, &value)); // Test if the read function returns false
    TEST_ASSERT_TRUE(circular_buffer_available(cbuffer, &value));
    TEST_ASSERT_EQUAL_UINT8(0U, value);
    TEST_ASSERT_TRUE(circular_buffer_isfull(cbuffer, &full));
    TEST_ASSERT_FALSE(full);

    // Test writing to an empty buffer
    TEST_ASSERT_TRUE(circular_buffer_write(cbuffer, 100U));
    TEST_ASSERT_TRUE(circular_buffer_available(cbuffer, &value));
    TEST_ASSERT_EQUAL_UINT8(1U, value);
    TEST_ASSERT_TRUE(circular_buffer_isfull(cbuffer, &full));
    TEST_ASSERT_FALSE(full);

    // Test reading from the buffer and again make it empty
    TEST_ASSERT_TRUE(circular_buffer_read(cbuffer, &value));
    TEST_ASSERT_EQUAL_UINT8(100U, value);
    TEST_ASSERT_TRUE(circular_buffer_available(cbuffer, &value));
    TEST_ASSERT_EQUAL_UINT8(0U, value);
    TEST_ASSERT_TRUE(circular_buffer_isfull(cbuffer, &full));
    TEST_ASSERT_FALSE(full);
}

void test_write_full_buffer(void)
{
    bool full = false;
    uint8_t value = 0U;

    write_buffer(1U, BUFSIZE); // Fill (BUFSIZE - 1) elements of the buffer with 1 to (BUFSIZE - 1)

    // Fill the last element of the buffer with BUFSIZE and make the buffer full
    TEST_ASSERT_TRUE(circular_buffer_write(cbuffer, BUFSIZE));
    TEST_ASSERT_TRUE(circular_buffer_available(cbuffer, &value));
    TEST_ASSERT_EQUAL_UINT8(BUFSIZE, value);
    TEST_ASSERT_TRUE(circular_buffer_isfull(cbuffer, &full));
    TEST_ASSERT_TRUE(full);

    // Test overwriting
    TEST_ASSERT_TRUE(circular_buffer_write(cbuffer, 100U));
    TEST_ASSERT_TRUE(circular_buffer_available(cbuffer, &value));
    TEST_ASSERT_EQUAL_UINT8(BUFSIZE, value);
    TEST_ASSERT_TRUE(circular_buffer_isfull(cbuffer, &full));
    TEST_ASSERT_TRUE(full);

    // Test peeking
    TEST_ASSERT_TRUE(circular_buffer_peek(cbuffer, &value));
    TEST_ASSERT_EQUAL_UINT8(2U, value);
    TEST_ASSERT_TRUE(circular_buffer_available(cbuffer, &value));
    TEST_ASSERT_EQUAL_UINT8(BUFSIZE, value);
    TEST_ASSERT_TRUE(circular_buffer_isfull(cbuffer, &full));
    TEST_ASSERT_TRUE(full);

    // Test reading
    TEST_ASSERT_TRUE(circular_buffer_read(cbuffer, &value));
    TEST_ASSERT_EQUAL_UINT8(2U, value);
    TEST_ASSERT_TRUE(circular_buffer_available(cbuffer, &value));
    TEST_ASSERT_EQUAL_UINT8(BUFSIZE - 1U, value);
    TEST_ASSERT_TRUE(circular_buffer_isfull(cbuffer, &full));
    TEST_ASSERT_FALSE(full);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_circular_buffer_create);
    RUN_TEST(test_read_write_buffer);
    RUN_TEST(test_reset_buffer);
    RUN_TEST(test_read_empty_buffer);
    RUN_TEST(test_read_full_buffer);
    RUN_TEST(test_write_empty_buffer);
    RUN_TEST(test_write_full_buffer);

    return UNITY_END();
}