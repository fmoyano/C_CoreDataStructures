#include "../../dyn_array.h"
#include <stdlib.h>

/* Deliberately unsafe examples; excluded from library tests. */
static void analyzer_use_after_free(void)
{
    int *p = malloc(sizeof(*p));

    if (p == NULL)
    {
        return;
    }

    free(p);

    *p = 42;
}

static void analyzer_use_after_free2(int release)
{
    int *p = malloc(sizeof(*p));

    if (p == NULL)
    {
        return;
    }

    if (release)
    {
        free(p);
    }

    *p = 42;

    if (!release)
    {
        free(p);
    }
}

static void analyzer_leak_demo1(void)
{
    int *numbers = array_allocate(4, sizeof(*numbers));

    if (numbers == NULL)
    {
        return;
    }

    numbers[0] = 42;

    /* Intentionally missing array_free(numbers); */
}

static void analyzer_no_leak_demo2(void)
{
    int *numbers = array_allocate(4, sizeof(*numbers));

    if (numbers == NULL)
    {
        return;
    }

    numbers[0] = 42;

    array_free(numbers);
}

int main(void)
{
    analyzer_use_after_free();
    analyzer_use_after_free2(1);
    analyzer_leak_demo1();
    analyzer_no_leak_demo2();
    return 0;
}
