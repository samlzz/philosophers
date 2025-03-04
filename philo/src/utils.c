/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:41:35 by sliziard          #+#    #+#             */
/*   Updated: 2025/03/04 17:10:37 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <sys/time.h>
#include <limits.h>

unsigned int ascii_to_uint(const char *nptr, int *error)
{
    unsigned long r;

    r = 0;
    *error = 0;
    if (!nptr || !*nptr)
    {
        *error = 1;
        return 0;
    }
    while (*nptr >= '0' && *nptr <= '9')
    {
        r = (r * 10) + (*nptr - '0');
        if (r > UINT_MAX)
        {
            *error = 1;
            return (UINT_MAX);
        }
        nptr++;
    }

    if (*nptr)
        *error = 1;
    return (unsigned int)r;
}

long	date_now()
{
	struct timeval tv;
	
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000);
}

void	philog(t_philo phi, t_paction state)
{
	char	*strs[ACT_DIE + 1];

	strs[ACT_FORK] = "has taken a fork";
	strs[ACT_EAT] = "is eating";
	strs[ACT_SLEEP] = "is sleeping";
	strs[ACT_THINK] = "is thinking";
	strs[ACT_DIE] = "is died";
	pthread_mutex_lock(&phi.data->print_mutex);
	printf("[%ld] %d %s\n", date_now(), phi.id, strs[state]);
	pthread_mutex_unlock(&phi.data->print_mutex);
}
