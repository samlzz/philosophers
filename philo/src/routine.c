/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 15:32:18 by sliziard          #+#    #+#             */
/*   Updated: 2025/02/28 17:59:08 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>

static void	_eat(t_philo *phi)
{
	phi->state = ACT_FORK;
	pthread_mutex_lock(phi->left_fork);
	philog(*phi);
	pthread_mutex_lock(phi->right_fork);
	philog(*phi);
	phi->state = ACT_EAT;
	philog(*phi);
	phi->meals_eaten++;
	usleep(phi->data->time_to_eat);
	pthread_mutex_unlock(phi->left_fork);
	pthread_mutex_unlock(phi->right_fork);
	phi->last_meal_time = date_now();
}

static void	_sleep(t_philo *phi)
{
	phi->state = ACT_SLEEP;
	philog(*phi);
	usleep(phi->data->time_to_sleep);
}

static void	_think(t_philo *phi)
{
	phi->state = ACT_THINK;
	philog(*phi);
}

static void	_check_death(t_philo *phi)
{
	size_t	i;
	bool	have_all_eat;

	if (date_now() - phi->data->start_time >= phi->data->time_to_die)
	{
		phi->state = ACT_DIE;
		philog(*phi);
		phi->data->sim_end = true;
	}
	if (phi->data->must_eat_count == -1)
		return ;
	i = 0;
	have_all_eat = true;
	while (i < phi->data->count)
	{
		if (phi->data->philos[i].meals_eaten < phi->data->must_eat_count)
		{
			have_all_eat = false;
			break ;
		}
		i++;
	}
	if (have_all_eat)
		phi->data->sim_end = true;
}

void	philo_life(t_philo *phi)
{
	while (!phi->data->sim_end)
	{
		_check_death(phi);
		_eat(phi);
		_check_death(phi);
		_sleep(phi);
		_check_death(phi);
		_think(phi);
	}
}