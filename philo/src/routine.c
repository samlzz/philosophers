/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 15:32:18 by sliziard          #+#    #+#             */
/*   Updated: 2025/03/04 17:51:08 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>

static void	_take_forks(t_philo p, t_mutex *one, t_mutex *two)
{	
	pthread_mutex_lock(one);
	philog(p, ACT_FORK);
	pthread_mutex_lock(two);
	philog(p, ACT_FORK);
}

static void	_eat(t_philo *phi)
{
	if (phi->id % 2)
		_take_forks(*phi, phi->left_fork, phi->right_fork);
	else
		_take_forks(*phi, phi->right_fork, phi->left_fork);
	pthread_mutex_lock(&phi->meal_mutex);
	philog(*phi, ACT_EAT);
	phi->last_meal_time = date_now();
	pthread_mutex_unlock(&phi->meal_mutex);
	phi->meals_eaten++;
	ft_usleep(phi->data->time_to_eat);
	pthread_mutex_unlock(phi->left_fork);
	pthread_mutex_unlock(phi->right_fork);
}

static void	_sleep(t_philo *phi)
{
	philog(*phi, ACT_SLEEP);
	ft_usleep(phi->data->time_to_sleep);
}

static void	_think(t_philo *phi)
{
	philog(*phi, ACT_THINK);
}

void	*philo_life(void *param)
{
	t_philo	*phi;

	phi = (t_philo *)param;
	while (!phi->data->sim_end)
	{
		_eat(phi);
		if (phi->data->sim_end)
			break ;
		_sleep(phi);
		if (phi->data->sim_end)
			break ;
		_think(phi);
	}
	return (NULL);
}