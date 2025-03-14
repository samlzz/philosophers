/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:41:35 by sliziard          #+#    #+#             */
/*   Updated: 2025/03/14 14:09:42 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>

uint64_t	date_now(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		write(2, "gettimeofday() error\n", 22);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000);
}

void	ft_usleep(size_t milliseconds)
{
	size_t	start;

	start = date_now();
	while ((date_now() - start) < milliseconds)
		usleep(USLEEP_INTERVAL);
}

void	philog(t_philo phi, t_paction state)
{
	long	time;
	char	*strs[ACT_DIE + 1];

	time = date_now();
	strs[ACT_FORK] = "has taken a fork";
	strs[ACT_EAT] = "is eating";
	strs[ACT_SLEEP] = "is sleeping";
	strs[ACT_THINK] = "is thinking";
	strs[ACT_DIE] = "died";
	pthread_mutex_lock(&phi.data->print_mutex);
	printf("%ld %d %s\n", time - phi.data->start_time, phi.id, strs[state]);
	pthread_mutex_unlock(&phi.data->print_mutex);
}

void	set_shared(t_shared	*__ptr, t_shared_ope ope, int32_t value)
{
	pthread_mutex_lock(&__ptr->mtx);
	if (ope == SH_SET)
		__ptr->__val = value;
	else if (ope == SH_INCREMENT)
		__ptr->__val += value;
	else if (ope == SH_DECREMENT)
		__ptr->__val -= value;
	pthread_mutex_unlock(&__ptr->mtx);
}

int32_t	get_shared(t_shared *from)
{
	int32_t	val;

	pthread_mutex_lock(&from->mtx);
	val = from->__val;
	pthread_mutex_unlock(&from->mtx);
	return (val);
}
