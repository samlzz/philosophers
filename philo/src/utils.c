/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:41:35 by sliziard          #+#    #+#             */
/*   Updated: 2025/03/04 17:48:58 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>

long	date_now()
{
	struct timeval tv;
	
	if (gettimeofday(&tv, NULL) == -1)
		write(2, "gettimeofday() error\n", 22);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000);
}

int	ft_usleep(size_t milliseconds)
{
	size_t	start;

	start = date_now();
	while ((date_now() - start) < milliseconds)
		usleep(500);
	return (0);
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
