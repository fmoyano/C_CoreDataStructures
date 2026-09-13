#ifndef INT_ARRAY_H
#define INT_ARRAY_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#define INITIAL_CAPACITY 4

typedef struct
{
    int* data;
    size_t size;
    size_t capacity;
} IntArray;


IntArray array_create(void)
{
    return (IntArray){
    .data = NULL,
    .size = 0,
    .capacity = 0
    };
}

static inline bool array_reserve(IntArray *array, size_t requested_capacity)
{
    if (requested_capacity <= array->capacity)
    {
        return true;
    }
    
    if (requested_capacity > SIZE_MAX / sizeof(*array->data))
    {
        return false;
    }

    int *new_data = realloc(array->data, requested_capacity * sizeof(*array->data));
    if (!new_data)
    {
        return false;
    }

    array->data = new_data;
    array->capacity = requested_capacity;

    return true;
}

static inline bool array_grow(IntArray *array)
{    
    size_t new_capacity;

    if (array->capacity == 0)
    {
        new_capacity = INITIAL_CAPACITY;
    }
    else
    {
        if (array->capacity > SIZE_MAX / 2)
        {
            return false;
        }

        new_capacity = array->capacity * 2;
    }

    return array_reserve(array, new_capacity);    
}

static inline bool array_push(IntArray *array, int value)
{
    if (array->size == array->capacity)
    {
        if (!array_grow(array))
        {
            return false;
        }
    }

    array->data[array->size] = value;
    array->size++;

    return true;
}

static inline void array_destroy(IntArray* array)
{
    free(array->data);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
}

static inline bool array_get(const IntArray *array, size_t index, int *out_value)
{
    if (index >= array->size || !out_value) {
        return false;
    }

    *out_value = array->data[index];
    return true;
}

static inline bool array_set(IntArray *array, size_t index, int value)
{
    if (index >= array->size) {
        return false;
    }

    array->data[index] = value;
    return true;
}

static inline bool array_pop(IntArray *array, int *out_value)
{
    if (array->size == 0)
    {
        return false;
    }
    
    --array->size;
    *out_value = array->data[array->size];
    
    return true;
}

static inline bool array_insert(IntArray *array, size_t index, int value)
{
    if (index > array->size)
    {
        return false;
    }

    if (array->size == array->capacity)
    {
        if (!array_grow(array))
        {
            return false;
        }
    }

    for (size_t i = array->size; i > index; --i)
    {
        array->data[i] = array->data[i - 1];
    }

    array->data[index] = value;
    ++array->size;

    return true;
}

static inline bool array_remove(IntArray *array, size_t index, int *out_value)
{
    if (index >= array->size)
    {
        return false;
    }

    int removed = array->data[index];

    for (size_t i = index + 1; i < array->size; ++i)
    {
        array->data[i - 1] = array->data[i];
    }
    
    --array->size;
    
    if (out_value)
    {
        *out_value = removed;
    }

    return true;
}

static void array_clear(IntArray* array)
{
    array->size = 0;
}

static inline bool array_shrink_to_fit(IntArray *array)
{
    if (array->size == array->capacity)
    {
        return true;
    }

    if (array->size > 0)
    {
        int* new_data = realloc(array->data, array->size * sizeof(*array->data));
        if (!new_data)
        {
            return false;
        }
        array->data = new_data;
    }
    else
    {
        free(array->data);
        array->data = NULL;
    }

    array->capacity = array->size;
    return true;
}


#endif