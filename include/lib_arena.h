/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib_arena.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel <sel@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:28:52 by sel               #+#    #+#             */
/*   Updated: 2025/08/29 15:39:48 by sel              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBARENA_H
#define LIBARENA_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/**
 * @file lib_arena.h
 * @brief LibArena: Lightweight C memory management library.
 *
 * LibArena provides tracked memory allocation with automatic cleanup,
 * custom finalizers, scoped allocation checkpoints, and safe realloc support.
 *
 * Example usage:
 *   Collector *c = arena_create(10);
 *   int *arr = arena_alloc(c, sizeof(int) * 5);
 *   arena_destroy(c);
 */

/**
 * @struct Collector
 * @brief Tracks allocated memory blocks and optional cleanup functions.
 *
 * - `addresses` stores pointers to allocated memory.
 * - `finalizers` stores optional cleanup functions for each pointer.
 * - `capacity` is the current maximum number of tracked pointers.
 * - `size` is the number of currently tracked allocations.
 */
typedef struct s_collector
{
    uintptr_t       *addresses;   /**< Array of allocated pointers */
    void            (**finalizers)(void*); /**< Optional cleanup functions */
    size_t          capacity;     /**< Maximum capacity of arrays */
    size_t          size;         /**< Current number of tracked allocations */
} Collector;

/* ==========================
   Core API
   ========================== */

/**
 * @brief Create a new memory collector.
 * @param initial_capacity Initial capacity for tracking allocations.
 * @return Pointer to a Collector instance, or NULL on allocation failure.
 */
Collector *arena_create(size_t initial_capacity);

/**
 * @brief Grow the internal tracking arrays of the collector.
 * @param c Collector instance.
 * @return 1 on success, 0 if memory allocation fails.
 *
 * The capacity is doubled, and existing allocations are preserved.
 */
int arena_grow(Collector *c);

/**
 * @brief Allocate memory and register it in the collector.
 * @param c Collector instance.
 * @param bytes Number of bytes to allocate.
 * @return Pointer to allocated memory, or NULL on failure.
 */
void *arena_alloc(Collector *c, size_t bytes);

/**
 * @brief Reallocate a tracked memory block and update the collector.
 * @param c Collector instance.
 * @param ptr Previously allocated pointer.
 * @param new_size New allocation size in bytes.
 * @return Pointer to reallocated memory, or NULL on failure.
 */
void *arena_realloc(Collector *c, void *ptr, size_t new_size);

/**
 * @brief Register a custom cleanup function for a pointer.
 * @param c Collector instance.
 * @param ptr Pointer to register.
 * @param finalize Function to call when the pointer is freed.
 *
 * Note: The finalizer should not free the pointer itself; LibArena
 *       handles freeing memory automatically.
 */
void arena_set_destructor(Collector *c, void *ptr, void (*finalize)(void*));

/**
 * @brief Free a single allocation tracked by the collector.
 * @param c Collector instance.
 * @param ptr Pointer to free.
 *
 * If a finalizer is registered for the pointer, it is called first.
 */
void arena_free(Collector *c, void *ptr);

/**
 * @brief Take a checkpoint to mark the current allocation state.
 * @param c Collector instance.
 * @return Checkpoint value representing the current size of the registry.
 *
 * Later, you can use `arena_restore` to free all allocations made
 * after this checkpoint.
 */
size_t arena_snapshot(Collector *c);

/**
 * @brief Free all allocations made after a checkpoint.
 * @param c Collector instance.
 * @param checkpoint Checkpoint value returned by `arena_snapshot`.
 *
 * Only allocations registered after the checkpoint are freed.
 * Allocations before the checkpoint remain untouched.
 */
void arena_restore(Collector *c, size_t checkpoint);

/**
 * @brief Free all tracked allocations and destroy the collector.
 * @param c Collector instance.
 *
 * All memory managed by the collector is freed, and any registered
 * finalizers are called. After this call, the collector pointer
 * must not be used.
 */
void arena_destroy(Collector *c);

#endif /* LIBARENA_H */
