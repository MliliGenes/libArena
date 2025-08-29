/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collector_finalizer.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel <sel@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:29:01 by sel               #+#    #+#             */
/*   Updated: 2025/08/29 14:42:38 by sel              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/lib_arena.h"

/* Register a cleanup function for a pointer */
void collector_register_finalizer(Collector *c, void *ptr, void (*finalize)(void*))
{
    size_t i;

    i = 0;
    while (i < c->size)
    {
        if ((void*)c->addresses[i] == ptr)
        {
            c->finalizers[i] = finalize;
            return ;
        }
        i++;
    }
}
