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
 * @param c The arena instance.
 * @param ptr The pointer to the memory to be freed.
 */
void arena_free(arena_t *c, void *ptr)
{
    size_t i;

    i = 0;
    while (i < c->size)
    {
        if ((void*)c->addresses[i] == ptr)
        {
            // If a finalizer exists, call it before freeing.
            if (c->finalizers[i])
                c->finalizers[i](ptr);

            // Free the memory and clear its tracking slot.
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
 * @param c The arena to be destroyed.
 */
void arena_destroy(arena_t *c)
{
    size_t i;

    if (!c)
        return;

    i = 0;
    while (i < c->size)
    {
        // Ensure the pointer has not already been freed.
        if (c->addresses[i] != 0)
        {
            // Call the finalizer if it exists.
            if (c->finalizers[i])
                c->finalizers[i]((void*)c->addresses[i]);

            // Free the tracked memory.
            free((void*)c->addresses[i]);
        }
        i++;
    }

    // Free the internal arrays and the collector itself.
    free(c->addresses);
    free(c->finalizers);
    free(c);
}
