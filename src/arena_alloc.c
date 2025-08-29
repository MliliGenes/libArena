/**
 * @file arena_alloc.c
 * @brief Memory allocation functions for the LibArena collector.
 * @date 2025-08-29
 */
#include "../include/lib_arena.h"

/**
 * @brief Allocates memory and registers it with the arena.
 *
 * This function performs the following steps:
 * 1. Checks if the arena has enough capacity. If not, it calls `arena_grow`
 *    to expand the internal tracking arrays.
 * 2. Allocates the requested number of bytes using `malloc`.
 * 3. If allocation is successful, it stores the new pointer in the `addresses`
 *    array and increments the arena's size.
 * 4. A `NULL` finalizer is set by default.
 *
 * @param c The arena collector instance.
 * @param bytes The number of bytes to allocate.
 * @return A pointer to the newly allocated memory, or `NULL` on failure.
 */
void* arena_alloc(Collector *c, size_t bytes)
{
    void *ptr;

    // If the arena is full, attempt to grow it.
    if (c->size == c->capacity)
    {
        if (!arena_grow(c))
            return (NULL);
    }
    // Allocate the requested memory.
    ptr = malloc(bytes);
    if (!ptr)
        return (NULL);

    // Register the new pointer in the arena.
    c->addresses[c->size] = (uintptr_t)ptr;
    c->finalizers[c->size] = NULL;
    c->size++;

    return (ptr);
}


