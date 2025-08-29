/**
 * @file lib_arena.h
 * @author sel (sel@student.42.fr)
 * @brief LibArena: A lightweight C memory arena library.
 * @date 2025-08-29
 *
 * @section description
 * LibArena provides a set of tools for arena-based memory management.
 * It simplifies memory tracking, helps prevent leaks, and offers features
 * like custom finalizers and scoped allocations.
 *
 * @section usage
 * Example usage:
 * @code
 * // Create an arena
 * Collector *arena = arena_create(10);
 *
 * // Allocate memory
 * int *arr = arena_alloc(arena, sizeof(int) * 5);
 *
 * // Destroy the arena and free all allocations
 * arena_destroy(arena);
 * @endcode
 */

#ifndef LIBARENA_H
#define LIBARENA_H

#include <stdint.h>
#include <stdlib.h>

/**
 * @struct Collector
 * @brief Manages a collection of memory allocations.
 *
 * The Collector is the core of the library, tracking pointers to allocated
 * memory, their corresponding finalizers, and the arena's capacity.
 *
 * @var Collector::addresses
 *      Array of pointers to the allocated memory blocks.
 * @var Collector::finalizers
 *      Array of function pointers for custom cleanup routines.
 * @var Collector::capacity
 *      The maximum number of allocations the arena can track.
 * @var Collector::size
 *      The current number of active allocations.
 * @since 1.0.0
 */
typedef struct s_collector
{
    uintptr_t       *addresses;
    void            (**finalizers)(void*);
    size_t          capacity;
    size_t          size;
} Collector;

/* ==========================
   Core API
   ========================== */

/**
 * @brief Creates a new memory arena.
 *
 * Initializes a `Collector` instance with a specified initial capacity for
 * tracking allocations.
 *
 * @param initial_capacity The starting number of allocations the arena can hold.
 * @return A pointer to the new `Collector`, or `NULL` if initialization fails.
 * @since 1.0.0
 */
Collector *arena_create(size_t initial_capacity);

/**
 * @brief Doubles the tracking capacity of the arena.
 *
 * This function is called internally by `arena_alloc` when the arena is full.
 * It reallocates the internal arrays (`addresses` and `finalizers`) to
 * accommodate more pointers.
 *
 * @param c A pointer to the `Collector` instance.
 * @return `1` on success, `0` if memory allocation fails.
 * @since 1.0.0
 */
int arena_grow(Collector *c);

/**
 * @brief Allocates memory and tracks it in the arena.
 *
 * If the arena is at capacity, it will attempt to grow before allocating.
 *
 * @param c A pointer to the `Collector` instance.
 * @param bytes The number of bytes to allocate.
 * @return A pointer to the allocated memory, or `NULL` on failure.
 * @since 1.0.0
 */
void *arena_alloc(Collector *c, size_t bytes);

/**
 * @brief Reallocates a tracked memory block.
 *
 * Safely reallocates a pointer that is already tracked by the arena,
 * updating the internal registry with the new address.
 *
 * @param c A pointer to the `Collector` instance.
 * @param ptr A pointer to a previously allocated block.
 * @param new_size The new size in bytes for the memory block.
 * @return A pointer to the reallocated memory, or `NULL` on failure.
 * @since 1.0.0
 */
void *arena_realloc(Collector *c, void *ptr, size_t new_size);

/**
 * @brief Registers a custom cleanup function for an allocation.
 *
 * The specified function will be called when the allocation is freed
 * by `arena_free` or `arena_destroy`.
 *
 * @note The finalizer should not free the pointer itself; the arena handles this.
 *
 * @param c A pointer to the `Collector` instance.
 * @param ptr The pointer to associate the finalizer with.
 * @param finalize The function to call upon cleanup.
 * @since 1.0.0
 */
void arena_set_destructor(Collector *c, void *ptr, void (*finalize)(void*));

/**
 * @brief Frees a single tracked allocation from the arena.
 *
 * If a finalizer is registered for the pointer, it is called before the
 * memory is freed.
 *
 * @param c A pointer to the `Collector` instance.
 * @param ptr The pointer to the memory to free.
 * @since 1.0.0
 */
void arena_free(Collector *c, void *ptr);

/**
 * @brief Creates a checkpoint of the current allocation state.
 *
 * Returns a snapshot of the arena's current size. This can be used with
 * `arena_restore` to implement scoped allocations.
 *
 * @param c A pointer to the `Collector` instance.
 * @return A checkpoint value representing the current number of allocations.
 * @since 1.0.0
 */
size_t arena_snapshot(Collector *c);

/**
 * @brief Frees all allocations made after a specific checkpoint.
 *
 * Rolls back the arena's state to a previous snapshot, freeing all memory
 * allocated since that point.
 *
 * @param c A pointer to the `Collector` instance.
 * @param checkpoint The checkpoint to restore to, obtained from `arena_snapshot`.
 * @since 1.0.0
 */
void arena_restore(Collector *c, size_t checkpoint);

/**
 * @brief Destroys the arena and frees all tracked allocations.
 *
 * This function iterates through all tracked pointers, calls any registered
 * finalizers, frees the memory, and finally frees the `Collector` itself.
 *
 * @param c A pointer to the `Collector` instance to be destroyed.
 * @since 1.0.0
 */
void arena_destroy(Collector *c);

#endif /* LIBARENA_H */
