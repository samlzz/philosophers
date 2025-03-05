/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:41:35 by sliziard          #+#    #+#             */
/*   Updated: 2025/03/05 14:29:18 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>

long	date_now(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		write(2, "gettimeofday() error\n", 22);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000);
}

int	ft_usleep(size_t milliseconds)
{
	size_t	start;

	start = date_now();
	while ((date_now() - start) < milliseconds)
		usleep(USLEEP_INTERVAL);
	return (0);
}

bool	philog(t_philo phi, t_paction state)
{
	long	timestamp;
	char	*strs[ACT_DIE + 1];

	strs[ACT_FORK] = "has taken a fork";
	strs[ACT_EAT] = "is eating";
	strs[ACT_SLEEP] = "is sleeping";
	strs[ACT_THINK] = "is thinking";
	strs[ACT_DIE] = "is died";
	timestamp = date_now();
	if (state != ACT_DIE && \
		timestamp - phi.last_meal_time >= phi.data->time_to_die)
		return (1);
	pthread_mutex_lock(&phi.data->print_mutex);
	printf("[%ld] %d %s\n", timestamp, phi.id, strs[state]);
	pthread_mutex_unlock(&phi.data->print_mutex);
	return (0);
}

void	set_sim_end(t_data *d_ptr, bool value)
{
	pthread_mutex_lock(&d_ptr->end_mutex);
	d_ptr->__sim_end = value;
	pthread_mutex_unlock(&d_ptr->end_mutex);
}

bool	get_sim_end(t_data *d_ptr)
{
	bool	end;

	pthread_mutex_lock(&d_ptr->end_mutex);
	end = d_ptr->__sim_end;
	pthread_mutex_unlock(&d_ptr->end_mutex);
	return (end);
}
