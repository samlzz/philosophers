/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:41:35 by sliziard          #+#    #+#             */
/*   Updated: 2025/03/25 22:48:55 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <stdio.h>
#include <sys/time.h>

int64_t	date_now(void)
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

void	philog(t_philo owner, t_paction state)
{
	long	time;
	char	*strs[ACT_DIE + 1];

	time = date_now() - owner.__dptr->start_time;
	strs[ACT_FORK] = "has taken a fork";
	strs[ACT_EAT] = "is eating";
	strs[ACT_SLEEP] = "is sleeping";
	strs[ACT_THINK] = "is thinking";
	strs[ACT_DIE] = "died";
	sem_wait(owner.__dptr->sem_print);
	printf("%ld %d %s\n", time, owner.id, strs[state]);
	sem_post(owner.__dptr->sem_print);
}

void	close_sems(t_data *d_ptr)
{
	if (d_ptr->sem_end)
		sem_close(d_ptr->sem_end);
	if (d_ptr->sem_start)
		sem_close(d_ptr->sem_start);
	if (d_ptr->sem_print)
		sem_close(d_ptr->sem_print);
	if (d_ptr->sem_sated)
		sem_close(d_ptr->sem_sated);
	if (d_ptr->forks)
		sem_close(d_ptr->forks);
}
