/**
 * @file arena_finalizer.c
 * @brief Functions for managing custom finalizers in the LibArena collector.
 * @date 2025-08-29
 */
#include "../include/lib_arena.h"

/**
 * @brief Associates a custom finalizer function with a tracked pointer.
 *
 * This function searches for the given pointer (`ptr`) in the arena's
 * `addresses` array. If found, it assigns the `finalize` function to the
 * corresponding slot in the `finalizers` array.
 *
 * The finalizer will be called when the pointer is freed via `arena_free`
 * or `arena_destroy`.
 *
 * @param c The arena collector instance.
 * @param ptr The pointer to which the finalizer should be attached.
 * @param finalize A function pointer to the custom cleanup routine.
 */
void arena_set_destructor(Collector *c, void *ptr, void (*finalize)(void*))
{
    size_t i;

    i = 0;
    while (i < c->size)
    {
        // Find the matching pointer in the registry.
        if ((void*)c->addresses[i] == ptr)
        {
            // Assign the finalizer to the corresponding slot.
            c->finalizers[i] = finalize;
            return;
        }
        i++;
    }
}
