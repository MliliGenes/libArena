/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collector_free.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel <sel@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:29:03 by sel               #+#    #+#             */
/*   Updated: 2025/08/29 15:27:14 by sel              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/lib_arena.h"

/* Free a single tracked allocation */
void arena_free(Collector *c, void *ptr)
{
    size_t i;

    i = 0;
    while (i < c->size)
    {
        if ((void*)c->addresses[i] == ptr)
        {
            /* Call finalizer for cleanup but do not free the pointer */
            if (c->finalizers[i])
                c->finalizers[i](ptr);
            free(ptr); /* LibArena always frees the memory */
            c->addresses[i] = 0;
            c->finalizers[i] = NULL;
            return ;
        }
        i++;
    }
}

/* Free all allocations and destroy collector */
void arena_destroy(Collector *c)
{
    size_t i;

    if (!c)
        return ;
    i = 0;
    while (i < c->size)
    {
        if (c->addresses[i] != 0)
        {
            /* Call finalizer for cleanup but LibArena frees memory */
            if (c->finalizers[i])
                c->finalizers[i]((void*)c->addresses[i]);
            free((void*)c->addresses[i]);
        }
        i++;
    }
    free(c->addresses);
    free(c->finalizers);
    free(c);
}
