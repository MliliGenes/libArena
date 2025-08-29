/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collector_alloc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel <sel@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:28:59 by sel               #+#    #+#             */
/*   Updated: 2025/08/29 14:59:26 by sel              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/lib_arena.h"

/* Allocate memory and register it in the collector */
void* collector_start(Collector *c, size_t bytes)
{
    void *ptr;

    if (c->size == c->capacity)
    {
        if (!collector_resize(c))
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


