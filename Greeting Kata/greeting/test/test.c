#include <unity.h>
#include <string.h>
#include <greeting.h>

void setUp(void) {}
void tearDown(void) {}

void test_empty_name()
{
    TEST_ASSERT_EQUAL_STRING("Hello, my friend.", greet(""));
    TEST_ASSERT_EQUAL_STRING("Hello, my friend.", greet("   $ $,   0, ."));
}

void test_single_normal_name()
{
    TEST_ASSERT_NULL(greet("Bobiiiiiiiiii"));
    TEST_ASSERT_EQUAL_STRING("Hello, Bob.", greet("Bob"));
}

void test_single_shouted_name()
{
    TEST_ASSERT_EQUAL_STRING("HELLO BOB!", greet("BOB"));
}

void test_empty_input(void)
{
    TEST_ASSERT_EQUAL_STRING("Hello, my friend.", greet(NULL));
    TEST_ASSERT_EQUAL_STRING("Hello, my friend.", greet(""));
    TEST_ASSERT_EQUAL_STRING("Hello, my friend.", greet("  ,    $"));
}

void test_one_normal_name(void)
{
    TEST_ASSERT_EQUAL_STRING("Hello, Bob.", greet("Bob"));
    TEST_ASSERT_EQUAL_STRING("Hello, Bob.", greet("      Bob"));
    TEST_ASSERT_EQUAL_STRING("Hello, Bob.", greet("  ,    Bob"));
    TEST_ASSERT_EQUAL_STRING("Hello, Bob.", greet("  ,    0Bob@"));
}

void test_one_uppercase_name(void)
{
    TEST_ASSERT_EQUAL_STRING("HELLO JERRY!", greet("JERRY"));
}

void test_invalid_input(void)
{
    TEST_ASSERT_NULL(greet("abcdabcdabcdabcdp")); // Name length is greater than NAME_MAX_LENGTH

    char text[(MAX_NAMES + 1U) * (NAME_MAX_LENGTH + 5U)] = {};
    for (uint8_t i = 0U; i <= MAX_NAMES; i++)
    {
        strcat(text, "Bob, ");
    }
    TEST_ASSERT_NULL(greet(text)); // Number of names is greater than MAX_NAMES
}

void test_two_normal_names(void)
{
    TEST_ASSERT_EQUAL_STRING("Hello, Jill and Jane.", greet("Jill, Jane"));
}

void test_multiple_normal_names(void)
{
    TEST_ASSERT_EQUAL_STRING("Hello, Amy, Brian, and Charlotte.", greet("Amy, Brian, Charlotte"));
    TEST_ASSERT_EQUAL_STRING("Hello, Amy, Brian, Charlotte, and Bob.", greet("Amy, Brian, Charlotte, Bob"));
}

void test_mixed_names(void)
{
    TEST_ASSERT_EQUAL_STRING("HELLO BOB AND JANE!", greet("BOB, JANE"));
    TEST_ASSERT_EQUAL_STRING("HELLO BRIAN, BOB, AND JANE!", greet("BRIAN, BOB, JANE"));
    TEST_ASSERT_EQUAL_STRING("Hello, Amy and Charlotte. AND HELLO BRIAN!", greet("Amy, BRIAN, Charlotte"));
    TEST_ASSERT_EQUAL_STRING("Hello, Amy, Charlotte, and Jill. AND HELLO BRIAN AND BOB!", greet("Amy, BRIAN, Charlotte, BOB, Jill"));
    TEST_ASSERT_EQUAL_STRING("Hello, Amy, Charlotte, and Jill. AND HELLO BRIAN, BOB, AND JANE!", greet("Amy, BRIAN, Charlotte, BOB, Jill, JANE"));
}

int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_empty_input);
    RUN_TEST(test_invalid_input);
    RUN_TEST(test_one_normal_name);
    RUN_TEST(test_one_uppercase_name);
    RUN_TEST(test_two_normal_names);
    RUN_TEST(test_multiple_normal_names);
    RUN_TEST(test_mixed_names);

    return UNITY_END();
}