/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 15:32:18 by sliziard          #+#    #+#             */
/*   Updated: 2025/03/02 19:28:43 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>

static void	_eat(t_philo *phi)
{	
	pthread_mutex_lock(phi->left_fork);
	philog(*phi, ACT_FORK);
	pthread_mutex_lock(phi->right_fork);
	philog(*phi, ACT_FORK);
	usleep(phi->data->time_to_eat * 1000);
	phi->meals_eaten++;
	phi->last_meal_time = date_now();
	philog(*phi, ACT_EAT);
	pthread_mutex_unlock(phi->left_fork);
	pthread_mutex_unlock(phi->right_fork);
}

static void	_sleep(t_philo *phi)
{
	philog(*phi, ACT_SLEEP);
	usleep(phi->data->time_to_sleep * 1000);
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