#ifndef DYN_ARRAY_H
#define DYN_ARRAY_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef union
{
    struct
    {
        size_t length;
        size_t capacity;  
    } metadata;
    
    max_align_t alignment;
} ArrayHeader;


static inline const ArrayHeader* array_header(const void* array)
{
    return (const ArrayHeader*)((const char*)array-sizeof(ArrayHeader));
}

static inline ArrayHeader* array_header_mut(void* array)
{
    return (ArrayHeader*)(( char*)array-sizeof(ArrayHeader));
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
    if (capacity == 0 || element_size == 0 ||
        capacity > (SIZE_MAX - sizeof(ArrayHeader)) / element_size)
    {
        return NULL;
    }

    void* total = malloc(sizeof(ArrayHeader) + capacity * element_size);
    if (!total)
    {
        return NULL;
    }

    ArrayHeader *header = total;
    header->metadata.length = 0;
    header->metadata.capacity = capacity;

    return (char*)total + sizeof(ArrayHeader);
}

#define array_reserve(array, requested_capacity)                                        \
    do                                                                                  \
    {                                                                                   \
        void *tmp = array_reserve_impl((array), (requested_capacity), sizeof(*(array)));    \
        if (tmp)                                                                        \
        {                                                                               \
            (array) = tmp;                                                              \
        }                                                                               \
    } while (0)


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
        return NULL;        
    }
    
    ArrayHeader* header = array_header_mut(array);
    ArrayHeader* new_header = realloc(header, sizeof(ArrayHeader) + requested_capacity * element_size);
    if (!new_header)
    {
        return NULL;         
    }
    
    new_header->metadata.capacity = requested_capacity;
    return new_header + 1;
}

#define array_free(array)           \
    do                              \
    {                               \
        array_free_impl((array));   \
        (array) = NULL;             \
    } while (0);
    

static inline void array_free_impl(void* array)
{
    if (array)
    {
        free(array_header_mut((array)));
    }
}

#endif