#include <assert.h>
#include "IntArray.h"

int main(void)
{
    IntArray array = array_create();

    /* Estado inicial */
    assert(array.data == NULL);
    assert(array.size == 0);
    assert(array.capacity == 0);

    /* push */
    assert(array_push(&array, 10));
    assert(array.size == 1);
    assert(array.capacity == INITIAL_CAPACITY);
    assert(array.data[0] == 10);

    assert(array_push(&array, 20));
    assert(array_push(&array, 30));
    assert(array_push(&array, 40));

    assert(array.size == 4);
    assert(array.data[0] == 10);
    assert(array.data[1] == 20);
    assert(array.data[2] == 30);
    assert(array.data[3] == 40);

    /* Este push fuerza crecimiento de 4 a 8 */
    assert(array_push(&array, 50));

    assert(array.size == 5);
    assert(array.capacity == 8);
    assert(array.data[4] == 50);

    /* get */
    int value;

    assert(array_get(&array, 2, &value));
    assert(value == 30);

    assert(!array_get(&array, 100, &value));
    assert(!array_get(&array, 0, NULL));

    /* set */
    assert(array_set(&array, 2, 99));
    assert(array.data[2] == 99);

    assert(!array_set(&array, 100, 123));

    /* insert en posición intermedia */
    assert(array_insert(&array, 2, 777));

    assert(array.size == 6);
    assert(array.data[0] == 10);
    assert(array.data[1] == 20);
    assert(array.data[2] == 777);
    assert(array.data[3] == 99);
    assert(array.data[4] == 40);
    assert(array.data[5] == 50);

    /* insert al principio */
    assert(array_insert(&array, 0, 111));

    assert(array.size == 7);
    assert(array.data[0] == 111);
    assert(array.data[1] == 10);

    /* insert al final */
    assert(array_insert(&array, array.size, 999));

    assert(array.size == 8);
    assert(array.data[7] == 999);

    /* índice inválido */
    assert(!array_insert(&array, array.size + 1, 123));

    /* remove */
    int removed;

    assert(array_remove(&array, 1, &removed));
    assert(removed == 10);

    assert(array.size == 7);
    assert(array.data[0] == 111);
    assert(array.data[1] == 20);
    assert(array.data[2] == 777);
    assert(array.data[3] == 99);
    assert(array.data[4] == 40);
    assert(array.data[5] == 50);
    assert(array.data[6] == 999);

    /* pop recuperando el valor */
    size_t old_size = array.size;

    assert(array_pop(&array, &removed));
    assert(removed == 999);
    assert(array.size == old_size - 1);

    /* pop descartando el valor */
    old_size = array.size;

    assert(array_pop(&array, NULL));
    assert(array.size == old_size - 1);

    /* Vaciar mediante pop */
    while (array.size > 0)
    {
        assert(array_pop(&array, NULL));
    }

    assert(array.size == 0);
    assert(!array_pop(&array, NULL));

    /* destroy */
    array_destroy(&array);

    assert(array.data == NULL);
    assert(array.size == 0);
    assert(array.capacity == 0);

    /* El array puede volver a reutilizarse después de destroy */
    assert(array_push(&array, 10));
    assert(array_push(&array, 20));
    assert(array_push(&array, 30));
    assert(array_push(&array, 40));
    assert(array_push(&array, 50));

    assert(array.size == 5);
    assert(array.capacity == 8);

    /* shrink_to_fit: size < capacity */
    assert(array_shrink_to_fit(&array));

    assert(array.size == 5);
    assert(array.capacity == 5);

    assert(array.data[0] == 10);
    assert(array.data[1] == 20);
    assert(array.data[2] == 30);
    assert(array.data[3] == 40);
    assert(array.data[4] == 50);

    /* shrink_to_fit: size == capacity; no debería hacer nada */
    assert(array_shrink_to_fit(&array));

    assert(array.size == 5);
    assert(array.capacity == 5);

    /* clear conserva la memoria reservada */
    array_clear(&array);

    assert(array.data != NULL);
    assert(array.size == 0);
    assert(array.capacity == 5);

    /* shrink_to_fit sobre un array vacío libera la memoria */
    assert(array_shrink_to_fit(&array));

    assert(array.data == NULL);
    assert(array.size == 0);
    assert(array.capacity == 0);

    /* Limpieza final; debe ser segura incluso estando ya vacío */
    array_destroy(&array);

    assert(array.data == NULL);
    assert(array.size == 0);
    assert(array.capacity == 0);

    return 0;
}
