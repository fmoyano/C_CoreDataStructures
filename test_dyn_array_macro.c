#include "dyn_array.h"
#include <assert.h>

int main(void)
{    
    int *numbers = array_allocate(4, sizeof(*numbers));

    assert(array_length(numbers) == 0);
    assert(array_capacity(numbers) == 4);

    array_free(numbers);

    assert(numbers == NULL);
    assert(array_length(numbers) == 0);
}