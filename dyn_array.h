#ifndef DYN_ARRAY_H
#define DYN_ARRAY_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef union
{
    struct
    {
        size_t length;
        size_t capacity;  
    } metadata;
    
    max_align_t alignment;
} ArrayHeader;


#define ARRAY_INITIAL_CAPACITY 4


static inline void array_allocation_failure(void)
{
    abort();
}

static inline const ArrayHeader* array_header(const void* array)
{
    return (const ArrayHeader*)((const char*)array - sizeof(ArrayHeader));
}

static inline ArrayHeader* array_header_mut(void* array)
{
    return (ArrayHeader*)((char*)array - sizeof(ArrayHeader));
}

static inline size_t array_length(const void* array)
{
    return array ? array_header(array)->metadata.length : 0;
}

static inline size_t array_capacity(const void* array)
{
    return array ? array_header(array)->metadata.capacity : 0;
}


static inline void* array_allocate(size_t capacity, size_t element_size)
{
    if (capacity == 0)
    {
        return NULL;
    }

    if (element_size == 0 || capacity > (SIZE_MAX - sizeof(ArrayHeader)) / element_size)
    {
        array_allocation_failure();
    }

    void* total = malloc(sizeof(ArrayHeader) + capacity * element_size);
    if (!total)
    {
        array_allocation_failure();
    }

    ArrayHeader *header = total;
    header->metadata.length = 0;
    header->metadata.capacity = capacity;

    return (char*)total + sizeof(ArrayHeader);
}

#define array_reserve(array, requested_capacity)    \
    ((array) = array_reserve_impl(                  \
        (array),                                    \
        (requested_capacity),                       \
        sizeof(*(array))                            \
    ))


static inline void *array_reserve_impl(void *array, size_t requested_capacity, size_t element_size)
{
    if (!array)
    {
        return array_allocate(requested_capacity, element_size);
    }

    if (requested_capacity <= array_capacity(array))
    {
        return array;
    }

    if (element_size == 0 || requested_capacity > (SIZE_MAX - sizeof(ArrayHeader)) / element_size)
    {
        array_allocation_failure();
    }
    
    ArrayHeader* header = array_header_mut(array);
    ArrayHeader* new_header = realloc(header, sizeof(ArrayHeader) + requested_capacity * element_size);
    if (!new_header)
    {
        array_allocation_failure();
    }
    
    new_header->metadata.capacity = requested_capacity;
    return new_header + 1;
}

#define array_free(array)           \
    do                              \
    {                               \
        array_free_impl((array));   \
        (array) = NULL;             \
    } while (0)
    

static inline void array_free_impl(void* array)
{
    if (array)
    {
        free(array_header_mut((array)));
    }
}

static inline void *array_grow_impl(void *array, size_t element_size)
{
    size_t current_capacity = array_capacity(array);

    if (current_capacity == 0)
    {
        return array_reserve_impl(
            array,
            4,
            element_size
        );
    }

    if (current_capacity > SIZE_MAX / 2)
    {
        array_allocation_failure();
    }

    size_t new_capacity = current_capacity * 2;

    return array_reserve_impl(
        array,
        new_capacity,
        element_size
    );
}

static inline void *array_ensure_capacity_for_push(void *array, size_t element_size)
{
    if (array_length(array) == array_capacity(array))
    {
        return array_grow_impl(array, element_size);
    }

    return array;
}


#define array_push(array, value) \
    (                            \
        (array) = array_ensure_capacity_for_push((array), sizeof(*(array))),  \
        (array)[array_header_mut((array))->metadata.length++] = (value)     \
    )

#define array_pop(array) ((array)[--array_header_mut((array))->metadata.length])

static inline void array_clear(void *array)
{
    if (array)
    {
        array_header_mut(array)->metadata.length = 0;
    }
}

#define array_insert(array, index, value)                               \
    (                                                                   \
        (array) = array_insert_impl((array), (index), sizeof(*(array))),\
        (array)[(index)] = (value),                                     \
        array_header_mut((array))->metadata.length++                    \
    )

static inline void *array_insert_impl(void *array, size_t index, size_t element_size)
{
    size_t length = array_length(array);
    if (index > length)
    {
        abort();
    }

    array = array_ensure_capacity_for_push(array, element_size);    
    char* data = array;
    memmove(data + (index + 1) * element_size, data + index * element_size, (length - index) * element_size);

    return array;
}


#define array_remove(array, index) array_remove_impl((array), (index), sizeof(*(array)))        

static inline void array_remove_impl(void *array, size_t index, size_t element_size)
{
    size_t length = array_length(array);
    if (index >= length)
    {
        abort();
    }

    char *data = array;
    memmove(data + index * element_size, data + (index + 1) * element_size, (length - index - 1) * element_size);

    array_header_mut(array)->metadata.length--;
}

#endif