/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collector_scope.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel <sel@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:29:09 by sel               #+#    #+#             */
/*   Updated: 2025/08/29 14:43:28 by sel              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/lib_arena.h"

/* Take a checkpoint of the current allocation state */
size_t collector_checkpoint(Collector *c)
{
    return (c->size);
}

/* Free all allocations made since a checkpoint */
void collector_scope_end(Collector *c, size_t checkpoint)
{
    size_t i;

    if (checkpoint > c->size)
        return ;
    i = checkpoint;
    while (i < c->size)
    {
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
    c->size = checkpoint;
}
