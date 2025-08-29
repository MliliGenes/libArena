/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collector_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel <sel@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:29:05 by sel               #+#    #+#             */
/*   Updated: 2025/08/29 15:08:24 by sel              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/lib_arena.h"

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

/* Internal helper: resize the collector's arrays */
int collector_resize(Collector *c)
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
    ft_memcpy(new_addr, c->addresses, c->size * sizeof(uintptr_t));
    ft_memcpy(new_fin, c->finalizers, c->size * sizeof(void(*)(void*)));
    free(c->addresses);
    free(c->finalizers);
    c->addresses = new_addr;
    c->finalizers = new_fin;
    c->capacity = new_capacity;
    return (1);
}

/* Initialize a new collector */
Collector* collector_init(size_t initial_capacity)
{
    Collector *c;

    c = malloc(sizeof(Collector));
    if (!c)
        return (NULL);
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
    memset(c->finalizers, 0, initial_capacity * sizeof(void(*)(void*)));
    return (c);
}
