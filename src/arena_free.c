/**
 * @file arena_free.c
 * @brief Functions for freeing and destroying arena allocations.
 * @date 2025-08-29
 */
#include "../include/lib_arena.h"

/**
 * @brief Frees a single tracked allocation from the arena.
 *
 * This function searches for the specified pointer in the arena. If found,
 * it performs the following actions:
 * 1. Calls the associated finalizer if one has been set.
 * 2. Frees the memory using `free`.
 * 3. Clears the entry in the `addresses` and `finalizers` arrays to prevent
 *    double-frees.
 *
 * @param c The arena collector instance.
 * @param ptr The pointer to the memory to be freed.
 */
void arena_free(Collector *c, void *ptr)
{
    size_t i;

    i = 0;
    while (i < c->size)
    {
        if ((void*)c->addresses[i] == ptr)
        {
            if (c->finalizers[i])
                c->finalizers[i](ptr);
            free(ptr);
            c->addresses[i] = 0;
            c->finalizers[i] = NULL;
            return;
        }
        i++;
    }
}

/**
 * @brief Frees all tracked allocations and destroys the collector.
 *
 * This function iterates through all tracked pointers in the arena,
 * calls any registered finalizers, and frees the associated memory.
 * Finally, it frees the internal tracking arrays and the collector struct itself.
 *
 * @param c The arena collector to be destroyed.
 */
void arena_destroy(Collector *c)
{
    size_t i;

    if (!c)
        return;

    i = 0;
    while (i < c->size)
    {
        if (c->addresses[i] != 0)
        {
            if (c->finalizers[i])
                c->finalizers[i]((void*)c->addresses[i]);
            free((void*)c->addresses[i]);
        }
        i++;
    }
    free(c->addresses);
    free(c->finalizers);
    free(c);
}
