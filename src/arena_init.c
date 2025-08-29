/**
 * @file arena_init.c
 * @brief Initialization and growth functions for the LibArena collector.
 * @date 2025-08-29
 */
#include "../include/lib_arena.h"

/**
 * @brief A local implementation of memcpy.
 *
 * Copies `n` bytes from memory area `src` to memory area `dest`.
 *
 * @param dest The destination buffer.
 * @param src The source buffer.
 * @param n The number of bytes to copy.
 * @return A pointer to the destination buffer `dest`.
 */
void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t				i;
	unsigned char		*d;
	const unsigned char	*s;

	if (dest == NULL || src == NULL)
		return (NULL);
	i = 0;
	d = (unsigned char *)dest;
	s = (const unsigned char *)src;
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (d);
}

/**
 * @brief Doubles the capacity of the arena's tracking arrays.
 *
 * This internal helper is called when the arena runs out of space. It allocates
 * new, larger arrays for addresses and finalizers, copies the old data,
 * and then frees the old arrays.
 *
 * @param c The arena collector instance.
 * @return 1 on success, 0 on memory allocation failure.
 */
int arena_grow(Collector *c)
{
    size_t new_capacity;
    uintptr_t *new_addr;
    void (**new_fin)(void*);
    
    new_capacity = c->capacity * 2;
    new_addr = malloc(new_capacity * sizeof(uintptr_t));
    new_fin = malloc(new_capacity * sizeof(void(*)(void*)));

    if (!new_addr || !new_fin)
    {
        free(new_addr);
        free(new_fin);
        return (0);
    }

    // Copy existing data to the new arrays.
    ft_memcpy(new_addr, c->addresses, c->size * sizeof(uintptr_t));
    ft_memcpy(new_fin, c->finalizers, c->size * sizeof(void(*)(void*)));

    // Free the old arrays.
    free(c->addresses);
    free(c->finalizers);

    // Update the collector with the new arrays and capacity.
    c->addresses = new_addr;
    c->finalizers = new_fin;
    c->capacity = new_capacity;

    return (1);
}

/**
 * @brief Initializes a new arena collector.
 *
 * Allocates memory for the collector struct and its internal tracking arrays.
 *
 * @param initial_capacity The number of pointers the arena can track initially.
 * @return A pointer to the new collector, or NULL if allocation fails.
 */
Collector* arena_create(size_t initial_capacity)
{
    Collector *c;

    c = malloc(sizeof(Collector));
    if (!c)
        return (NULL);

    // Allocate memory for the tracking arrays.
    c->addresses = malloc(initial_capacity * sizeof(uintptr_t));
    c->finalizers = malloc(initial_capacity * sizeof(void(*)(void*)));

    if (!c->addresses || !c->finalizers)
    {
        free(c->addresses);
        free(c->finalizers);
        free(c);
        return (NULL);
    }

    c->capacity = initial_capacity;
    c->size = 0;

    // Initialize finalizers array to NULLs.
    memset(c->finalizers, 0, initial_capacity * sizeof(void(*)(void*)));

    return (c);
}
