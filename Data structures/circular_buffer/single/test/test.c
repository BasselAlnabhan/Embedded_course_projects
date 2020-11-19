#include <unity.h>
#include <circular_buffer.h>

void setUp()
{
    circular_buffer_init();
}

void tearDown()
{
}

void test_circular_buffer_init(void)
{
    circular_buffer_write(100U);
    TEST_ASSERT_EQUAL_UINT8(1U, circular_buffer_available());
    TEST_ASSERT_FALSE(circular_buffer_isfull());

    circular_buffer_write(110U);
    TEST_ASSERT_EQUAL_UINT8(2U, circular_buffer_available());
    TEST_ASSERT_FALSE(circular_buffer_isfull());

    TEST_ASSERT_EQUAL_UINT8(100U, circular_buffer_read());
    TEST_ASSERT_EQUAL_UINT8(1U, circular_buffer_available());
    TEST_ASSERT_FALSE(circular_buffer_isfull());

    circular_buffer_init();

    TEST_ASSERT_FALSE(circular_buffer_isfull());
    TEST_ASSERT_EQUAL_UINT8(0U, circular_buffer_available());
}

void test_read_write_buffer(void)
{
    for (uint8_t i = 0U; i < BUFFER_SIZE - 3U; i++)
    {
        circular_buffer_write(i + 1);
        TEST_ASSERT_EQUAL_UINT8(i + 1, circular_buffer_available());
        TEST_ASSERT_FALSE(circular_buffer_isfull());
    }

    TEST_ASSERT_EQUAL_UINT8(1U, circular_buffer_peek());
    TEST_ASSERT_EQUAL_UINT8(BUFFER_SIZE - 3U, circular_buffer_available());
    TEST_ASSERT_FALSE(circular_buffer_isfull());

    TEST_ASSERT_EQUAL_UINT8(1U, circular_buffer_read());
    TEST_ASSERT_EQUAL_UINT8(BUFFER_SIZE - 4U, circular_buffer_available());
    TEST_ASSERT_FALSE(circular_buffer_isfull());

    TEST_ASSERT_EQUAL_UINT8(2U, circular_buffer_read());
    TEST_ASSERT_EQUAL_UINT8(BUFFER_SIZE - 5U, circular_buffer_available());
    TEST_ASSERT_FALSE(circular_buffer_isfull());

    TEST_ASSERT_EQUAL_UINT8(3U, circular_buffer_read());
    TEST_ASSERT_EQUAL_UINT8(BUFFER_SIZE - 6U, circular_buffer_available());
    TEST_ASSERT_FALSE(circular_buffer_isfull());

    for (uint8_t i = 0U; i < 4U; i++)
    {
        circular_buffer_write(i + 1U);
        TEST_ASSERT_EQUAL_UINT8(BUFFER_SIZE - 5U + i, circular_buffer_available());
        TEST_ASSERT_FALSE(circular_buffer_isfull());
    }

    TEST_ASSERT_EQUAL_UINT8(4U, circular_buffer_read());
    TEST_ASSERT_EQUAL_UINT8(BUFFER_SIZE - 3U, circular_buffer_available());
    TEST_ASSERT_FALSE(circular_buffer_isfull());
}

void test_read_empty_buffer(void)
{
    circular_buffer_read();
    TEST_ASSERT_FALSE(circular_buffer_isfull());
    TEST_ASSERT_EQUAL_UINT8(0U, circular_buffer_available());

    circular_buffer_write(100U);
    TEST_ASSERT_EQUAL_UINT8(1U, circular_buffer_available());
    TEST_ASSERT_FALSE(circular_buffer_isfull());

    circular_buffer_write(110U);
    TEST_ASSERT_EQUAL_UINT8(2U, circular_buffer_available());
    TEST_ASSERT_FALSE(circular_buffer_isfull());

    TEST_ASSERT_EQUAL_UINT8(100U, circular_buffer_read());
    TEST_ASSERT_EQUAL_UINT8(1U, circular_buffer_available());
    TEST_ASSERT_FALSE(circular_buffer_isfull());

    TEST_ASSERT_EQUAL_UINT8(110U, circular_buffer_read());
    TEST_ASSERT_EQUAL_UINT8(0U, circular_buffer_available());
    TEST_ASSERT_FALSE(circular_buffer_isfull());

    circular_buffer_read();
    TEST_ASSERT_FALSE(circular_buffer_isfull());
    TEST_ASSERT_EQUAL_UINT8(0U, circular_buffer_available());

    circular_buffer_write(100U);
    TEST_ASSERT_EQUAL_UINT8(100U, circular_buffer_read());
    TEST_ASSERT_EQUAL_UINT8(0U, circular_buffer_available());
    TEST_ASSERT_FALSE(circular_buffer_isfull());
}

void test_read_full_buffer(void)
{
    for (uint8_t i = 0U; i < BUFFER_SIZE; i++)
    {
        circular_buffer_write(i + 1);
        TEST_ASSERT_EQUAL_UINT8(i + 1, circular_buffer_available());
    }
    TEST_ASSERT_TRUE(circular_buffer_isfull());
    TEST_ASSERT_EQUAL_UINT8(BUFFER_SIZE, circular_buffer_available());

    TEST_ASSERT_EQUAL_UINT8(1U, circular_buffer_peek());
    TEST_ASSERT_EQUAL_UINT8(BUFFER_SIZE, circular_buffer_available());
    TEST_ASSERT_TRUE(circular_buffer_isfull());

    TEST_ASSERT_EQUAL_UINT8(1U, circular_buffer_read());
    TEST_ASSERT_EQUAL_UINT8(BUFFER_SIZE - 1U, circular_buffer_available());
    TEST_ASSERT_FALSE(circular_buffer_isfull());
}

void test_write_empty_buffer(void)
{
    circular_buffer_write(1U);
    TEST_ASSERT_EQUAL_UINT8(1U, circular_buffer_available());
    TEST_ASSERT_FALSE(circular_buffer_isfull());

    TEST_ASSERT_EQUAL_UINT8(1U, circular_buffer_read());
    TEST_ASSERT_EQUAL_UINT8(0U, circular_buffer_available());
    TEST_ASSERT_FALSE(circular_buffer_isfull());

    circular_buffer_write(1U);
    TEST_ASSERT_EQUAL_UINT8(1U, circular_buffer_available());
    TEST_ASSERT_FALSE(circular_buffer_isfull());

    TEST_ASSERT_EQUAL_UINT8(1U, circular_buffer_read());
    TEST_ASSERT_EQUAL_UINT8(0U, circular_buffer_available());
    TEST_ASSERT_FALSE(circular_buffer_isfull());
}

void test_write_full_buffer(void)
{
    for (uint8_t i = 0U; i < BUFFER_SIZE; i++)
    {
        circular_buffer_write(i + 1U);
        TEST_ASSERT_EQUAL_UINT8(i + 1U, circular_buffer_available());
    }
    TEST_ASSERT_TRUE(circular_buffer_isfull());
    TEST_ASSERT_EQUAL_UINT8(BUFFER_SIZE, circular_buffer_available());

    circular_buffer_write(0U);
    TEST_ASSERT_TRUE(circular_buffer_isfull());
    TEST_ASSERT_EQUAL_UINT8(BUFFER_SIZE, circular_buffer_available());

    TEST_ASSERT_EQUAL_UINT8(2U, circular_buffer_peek());
    TEST_ASSERT_EQUAL_UINT8(BUFFER_SIZE, circular_buffer_available());
    TEST_ASSERT_TRUE(circular_buffer_isfull());

    TEST_ASSERT_EQUAL_UINT8(2U, circular_buffer_read());
    TEST_ASSERT_EQUAL_UINT8(BUFFER_SIZE - 1U, circular_buffer_available());
    TEST_ASSERT_FALSE(circular_buffer_isfull());
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_circular_buffer_init);
    RUN_TEST(test_read_write_buffer);
    RUN_TEST(test_read_empty_buffer);
    RUN_TEST(test_read_full_buffer);
    RUN_TEST(test_write_empty_buffer);
    RUN_TEST(test_write_full_buffer);

    return UNITY_END();
}