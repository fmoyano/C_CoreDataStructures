#include "dyn_array.h"
#include "unity.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_array_allocate_and_free(void)
{
    int *numbers = array_allocate(4, sizeof(*numbers));

    TEST_ASSERT_EQUAL_size_t(0, array_length(numbers));
    TEST_ASSERT_EQUAL_size_t(3, array_capacity(numbers));

    array_free(numbers);

    TEST_ASSERT_NULL(numbers);
    TEST_ASSERT_EQUAL_size_t(0, array_capacity(numbers));
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_array_allocate_and_free);

    return UNITY_END();
}