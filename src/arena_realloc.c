/**
 * @file arena_realloc.c
 * @brief Memory reallocation function for the LibArena collector.
 * @date 2025-08-29
 */
#include "../include/lib_arena.h"

/**
 * @brief Reallocates a tracked memory block and updates the arena.
 *
 * This function handles several cases:
 * 1. If `ptr` is `NULL`, it behaves like `arena_alloc`.
 * 2. It searches for `ptr` in the arena. If not found, it returns `NULL`.
 * 3. If `ptr` is found, it calls `realloc` to resize the memory block.
 * 4. On successful reallocation, it updates the corresponding address in the
 *    arena's tracking array.
 *
 * @param c The arena collector instance.
 * @param ptr The pointer to the memory block to reallocate. Can be `NULL`.
 * @param new_size The new size for the memory block.
 * @return A pointer to the reallocated memory, or `NULL` on failure.
 */
void* arena_realloc(Collector *c, void *ptr, size_t new_size)
{
    size_t i;
    void *new_ptr;

    if (!ptr)
        return (arena_alloc(c, new_size));
    i = 0;
    while (i < c->size)
    {
        if ((void*)c->addresses[i] == ptr)
            break;
        i++;
    }
    if (i == c->size)
        return (NULL);
    new_ptr = realloc(ptr, new_size);
    if (!new_ptr)
        return (NULL);
    c->addresses[i] = (uintptr_t)new_ptr;
    return (new_ptr);
}
