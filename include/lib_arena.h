/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib_arena.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel <sel@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:28:52 by sel               #+#    #+#             */
/*   Updated: 2025/08/29 14:52:02 by sel              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBARENA_H
#define LIBARENA_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/**
 * @file libarena.h
 * @brief 42-norm-compliant C memory collector library.
 *
 * Provides:
 *  - Allocation tracking with automatic cleanup.
 *  - Scoped checkpoints for rollback.
 *  - Reallocation support.
 *  - Custom finalizers for nested resources.
 *  - Auto-resizing internal registry.
 *
 * Example usage:
 *  Collector *c = collector_init(4);
 *  int *x = collector_start(c, sizeof(int));
 *  collector_register_finalizer(c, x, my_cleanup);
 *  size_t cp = collector_checkpoint(c);
 *  collector_scope_end(c, cp);
 *  collector_end(c);
 */

/**
 * @struct Collector
 * @brief Tracks allocated pointers and optional finalizers.
 */
typedef struct s_collector
{
    uintptr_t *addresses;       /**< Array of allocated pointers */
    void (**finalizers)(void*); /**< Array of optional finalizers */
    size_t capacity;            /**< Maximum capacity of arrays */
    size_t size;                /**< Current number of tracked allocations */
} Collector;

/* ==========================
   Core API
   ========================== */

/**
 * @brief Initialize a new collector.
 * @param initial_capacity Initial size of internal arrays.
 * @return Pointer to a Collector instance, or NULL on failure.
 */
Collector* collector_init(size_t initial_capacity);

/**
 * @brief Resize the internal arrays of the collector.
 * @param c Collector instance.
 * @return 1 on success, 0 on allocation failure.
 *
 * This function doubles the capacity of the collector's internal arrays.
 * It preserves existing tracked pointers and finalizers.
 */
int collector_resize(Collector *c);

/**
 * @brief Allocate memory and register it in the collector.
 * @param c Collector instance.
 * @param bytes Number of bytes to allocate.
 * @return Pointer to allocated memory, or NULL on failure.
 */
void* collector_start(Collector *c, size_t bytes);

/**
 * @brief Reallocate a tracked pointer and update the registry.
 * @param c Collector instance.
 * @param ptr Pointer previously allocated with collector_start.
 * @param new_size New size in bytes.
 * @return Pointer to reallocated memory, or NULL on failure.
 */
void* collector_realloc(Collector *c, void *ptr, size_t new_size);

/**
 * @brief Register a cleanup function (finalizer) for a pointer.
 * @param c Collector instance.
 * @param ptr Pointer to register.
 * @param finalize Function to call on free.
 */
void collector_register_finalizer(Collector *c, void *ptr, void (*finalize)(void*));

/**
 * @brief Free a single allocation tracked by the collector.
 * @param c Collector instance.
 * @param ptr Pointer to free.
 */
void collector_free_ptr(Collector *c, void *ptr);

/**
 * @brief Take a checkpoint to mark the current allocation state.
 * @param c Collector instance.
 * @return Checkpoint value (current size of registry).
 */
size_t collector_checkpoint(Collector *c);

/**
 * @brief Free all allocations made since a checkpoint.
 * @param c Collector instance.
 * @param checkpoint Checkpoint value obtained earlier.
 */
void collector_scope_end(Collector *c, size_t checkpoint);

/**
 * @brief Free all allocations and destroy the collector.
 * @param c Collector instance.
 */
void collector_end(Collector *c);

#endif /* LIBARENA_H */
