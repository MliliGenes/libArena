/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena_realloc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel <sel@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:29:07 by sel               #+#    #+#             */
/*   Updated: 2025/08/29 15:25:56 by sel              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/lib_arena.h"

/* Reallocate memory and update the registry */
void* arena_realloc(Collector *c, void *ptr, size_t new_size)
{
    size_t i;
    void *new_ptr;

    if (!ptr)
        return (arena_alloc(c, new_size));
    i = 0;
    while (i < c->size)
    {
        if ((void*)c->addresses[i] == ptr)
            break ;
        i++;
    }
    if (i == c->size)
        return (NULL);
    new_ptr = realloc(ptr, new_size);
    if (!new_ptr)
        return (NULL);
    c->addresses[i] = (uintptr_t)new_ptr;
    return (new_ptr);
}
