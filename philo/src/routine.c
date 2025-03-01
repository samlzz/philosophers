/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 15:32:18 by sliziard          #+#    #+#             */
/*   Updated: 2025/03/01 15:40:34 by sliziard         ###   ########.fr       */
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

void	philo_life(t_philo *phi)
{
	while (!phi->data->sim_end)
	{
		_eat(phi);
		if (phi->data->sim_end)
			break ;
		_sleep(phi);
		if (phi->data->sim_end)
			break ;
		_think(phi);
		if (phi->data->sim_end)
			break ;
	}
}