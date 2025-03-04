/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 15:32:18 by sliziard          #+#    #+#             */
/*   Updated: 2025/03/04 22:37:12 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>

static bool	_eat(t_philo *phi)
{
	pthread_mutex_lock(phi->left_fork);
	if (philog(*phi, ACT_FORK) || phi->left_fork == phi->right_fork)
		return (pthread_mutex_unlock(phi->left_fork), 1);
	pthread_mutex_lock(phi->right_fork);
	if (philog(*phi, ACT_FORK))
	{
		pthread_mutex_unlock(phi->left_fork);
		pthread_mutex_unlock(phi->right_fork);
		return (1);
	}
	pthread_mutex_lock(&phi->meal_mutex);
	phi->last_meal_time = date_now();
	phi->meals_eaten++;
	pthread_mutex_unlock(&phi->meal_mutex);
	philog(*phi, ACT_EAT);
	ft_usleep(phi->data->time_to_eat);
	pthread_mutex_unlock(phi->left_fork);
	pthread_mutex_unlock(phi->right_fork);
	if (phi->data->must_eat_count == -1)
		return (0);
	return (phi->meals_eaten >= phi->data->must_eat_count);
}

static bool	_sleep(t_philo *phi)
{
	bool	exit;

	exit = philog(*phi, ACT_SLEEP);
	if (exit)
		return (exit);
	ft_usleep(phi->data->time_to_sleep);
	return (0);
}

static bool	_think(t_philo *phi)
{
	bool	exit;

	exit = philog(*phi, ACT_THINK);
	if (exit)
		return (exit);
	usleep(50);
	return (0);
}

void	*philo_life(void *param)
{
	t_philo	*phi;

	phi = (t_philo *)param;
	if (phi->id % 2 == 1)
		usleep(200);
	while (!get_sim_end(phi->data))
	{
		if (_eat(phi) || get_sim_end(phi->data))
			break ;
		if (_sleep(phi) || get_sim_end(phi->data))
			break ;
		if (_think(phi))
			break ;
	}
	return (NULL);
}
