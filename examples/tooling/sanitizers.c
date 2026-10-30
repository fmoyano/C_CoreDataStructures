#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

/* Run one deliberately invalid example per process. */
void test_ubsan_signed_overflow(void)
{
    volatile int value = INT_MAX;

    value += 1;
}

void test_asan_use_after_free(void)
{
    int *p = malloc(sizeof(*p));

    if (p == NULL)
    {
        exit(EXIT_FAILURE);
    }

    free(p);

    *p = 42;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s asan|ubsan\n", argv[0]);
        return EXIT_FAILURE;
    }
    if (strcmp(argv[1], "asan") == 0)
    {
        test_asan_use_after_free();
    }
    else if (strcmp(argv[1], "ubsan") == 0)
    {
        test_ubsan_signed_overflow();
    }
    else
    {
        fprintf(stderr, "Unknown example: %s\n", argv[1]);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
