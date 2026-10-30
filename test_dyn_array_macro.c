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
    TEST_ASSERT_EQUAL_size_t(4, array_capacity(numbers));

    array_free(numbers);

    TEST_ASSERT_NULL(numbers);
    TEST_ASSERT_EQUAL_size_t(0, array_capacity(numbers));
}

void test_array_push_from_null(void)
{
    int *numbers = NULL;

    array_push(numbers, 10);

    TEST_ASSERT_NOT_NULL(numbers);
    TEST_ASSERT_EQUAL_size_t(1, array_length(numbers));
    TEST_ASSERT_EQUAL_size_t(4, array_capacity(numbers));
    TEST_ASSERT_EQUAL_INT(10, numbers[0]);

    array_free(numbers);
}

void test_array_push_without_growth(void)
{
    int *numbers = NULL;

    array_push(numbers, 10);
    array_push(numbers, 20);
    array_push(numbers, 30);
    array_push(numbers, 40);

    TEST_ASSERT_EQUAL_size_t(4, array_length(numbers));
    TEST_ASSERT_EQUAL_size_t(4, array_capacity(numbers));

    TEST_ASSERT_EQUAL_INT(10, numbers[0]);
    TEST_ASSERT_EQUAL_INT(20, numbers[1]);
    TEST_ASSERT_EQUAL_INT(30, numbers[2]);
    TEST_ASSERT_EQUAL_INT(40, numbers[3]);

    array_free(numbers);
}

void test_array_push_grows_when_full(void)
{
    int *numbers = NULL;

    array_push(numbers, 10);
    array_push(numbers, 20);
    array_push(numbers, 30);
    array_push(numbers, 40);

    TEST_ASSERT_EQUAL_size_t(4, array_capacity(numbers));

    array_push(numbers, 50);

    TEST_ASSERT_EQUAL_size_t(5, array_length(numbers));
    TEST_ASSERT_EQUAL_size_t(8, array_capacity(numbers));

    TEST_ASSERT_EQUAL_INT(10, numbers[0]);
    TEST_ASSERT_EQUAL_INT(20, numbers[1]);
    TEST_ASSERT_EQUAL_INT(30, numbers[2]);
    TEST_ASSERT_EQUAL_INT(40, numbers[3]);
    TEST_ASSERT_EQUAL_INT(50, numbers[4]);

    array_free(numbers);
}

void test_array_pop(void)
{
    int *numbers = NULL;

    array_push(numbers, 10);
    array_push(numbers, 20);
    array_push(numbers, 30);

    int value = array_pop(numbers);

    TEST_ASSERT_EQUAL_INT(30, value);
    TEST_ASSERT_EQUAL_size_t(2, array_length(numbers));
    TEST_ASSERT_EQUAL_size_t(4, array_capacity(numbers));

    array_free(numbers);
}

void test_array_clear(void)
{
    int *numbers = NULL;

    array_push(numbers, 10);
    array_push(numbers, 20);

    size_t capacity_before = array_capacity(numbers);

    array_clear(numbers);

    TEST_ASSERT_EQUAL_size_t(0, array_length(numbers));
    TEST_ASSERT_EQUAL_size_t(capacity_before, array_capacity(numbers));

    array_free(numbers);
}

void test_array_insert(void)
{
    int *numbers = NULL;

    array_push(numbers, 10);
    array_push(numbers, 20);
    array_push(numbers, 30);

    array_insert(numbers, 1, 15);

    TEST_ASSERT_EQUAL_size_t(4, array_length(numbers));
    TEST_ASSERT_EQUAL_INT(10, numbers[0]);
    TEST_ASSERT_EQUAL_INT(15, numbers[1]);
    TEST_ASSERT_EQUAL_INT(20, numbers[2]);
    TEST_ASSERT_EQUAL_INT(30, numbers[3]);

    array_insert(numbers, 0, 5);
    TEST_ASSERT_EQUAL_INT(5, numbers[0]);

    array_insert(numbers, array_length(numbers), 40);
    TEST_ASSERT_EQUAL_INT(40, numbers[array_length(numbers) - 1]);

    array_free(numbers);
}

void test_array_remove(void)
{
    int *numbers = NULL;

    array_push(numbers, 10);
    array_push(numbers, 20);
    array_push(numbers, 30);
    array_push(numbers, 40);

    array_remove(numbers, 1);

    TEST_ASSERT_EQUAL_size_t(3, array_length(numbers));
    TEST_ASSERT_EQUAL_INT(10, numbers[0]);
    TEST_ASSERT_EQUAL_INT(30, numbers[1]);
    TEST_ASSERT_EQUAL_INT(40, numbers[2]);

    array_remove(numbers, 0);
    TEST_ASSERT_EQUAL_INT(30, numbers[0]);

    array_remove(numbers, array_length(numbers) - 1);
    TEST_ASSERT_EQUAL_size_t(1, array_length(numbers));

    array_free(numbers);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_array_allocate_and_free);
    RUN_TEST(test_array_push_from_null);
    RUN_TEST(test_array_push_without_growth);
    RUN_TEST(test_array_push_grows_when_full);
    RUN_TEST(test_array_pop);
    RUN_TEST(test_array_clear);
    RUN_TEST(test_array_insert);
    RUN_TEST(test_array_remove);

    return UNITY_END();
}
