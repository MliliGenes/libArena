/**
 * @file arena_scope.c
 * @brief Functions for managing scoped allocations in the LibArena collector.
 * @date 2025-08-29
 */
#include "../include/lib_arena.h"

/**
 * @brief Captures the current state of the arena by recording its size.
 *
 * This function returns the current number of allocations, which can be used
 * as a "checkpoint" for later restoration.
 *
 * @param c The arena collector instance.
 * @return The current size of the arena, representing the checkpoint.
 */
size_t arena_snapshot(Collector *c)
{
    return (c->size);
}

/**
 * @brief Frees all allocations made after a given checkpoint.
 *
 * This function iterates backward from the current size of the arena to the
 * checkpoint, freeing each allocation it encounters. It is useful for

 * implementing scoped memory that is automatically cleaned up when leaving
 * a certain block of code.
 *
 * @param c The arena collector instance.
 * @param checkpoint A checkpoint value obtained from `arena_snapshot`.
 */
void arena_restore(Collector *c, size_t checkpoint)
{
    size_t i;

    // Do nothing if the checkpoint is invalid.
    if (checkpoint > c->size)
        return;

    i = checkpoint;
    while (i < c->size)
    {
        // Free the pointer if it hasn't been freed already.
        if (c->addresses[i] != 0)
        {
            if (c->finalizers[i])
                c->finalizers[i]((void*)c->addresses[i]);
            free((void*)c->addresses[i]);
            c->addresses[i] = 0;
            c->finalizers[i] = NULL;
        }
        i++;
    }

    // Reset the size of the arena to the checkpoint.
    c->size = checkpoint;
}
