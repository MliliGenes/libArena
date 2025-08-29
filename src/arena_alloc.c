/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collector_alloc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel <sel@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:28:59 by sel               #+#    #+#             */
/*   Updated: 2025/08/29 15:27:25 by sel              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/lib_arena.h"

/* Allocate memory and register it in the collector */
void* arena_alloc(Collector *c, size_t bytes)
{
    void *ptr;

    if (c->size == c->capacity)
    {
        if (!arena_grow(c))
            return (NULL);
    }
    ptr = malloc(bytes);
    if (!ptr)
        return (NULL);
    c->addresses[c->size] = (uintptr_t)ptr;
    c->finalizers[c->size] = NULL;
    c->size++;
    return (ptr);
}


