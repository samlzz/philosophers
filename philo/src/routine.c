/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 15:32:18 by sliziard          #+#    #+#             */
/*   Updated: 2025/03/04 21:28:27 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>

static bool	_take_forks(t_philo p, t_mutex *one, t_mutex *two)
{
	bool	exit;

	exit = 0;
	pthread_mutex_lock(one);
	if (philog(p, ACT_FORK))
		exit = 1;
	if (one == two)
		return (pthread_mutex_unlock(one), 1);
	pthread_mutex_lock(two);
	if (philog(p, ACT_FORK))
		exit = 1;
	if (exit)
	{
		pthread_mutex_unlock(one);
		pthread_mutex_unlock(two);
	}
	return (exit);
}

static bool	_eat(t_philo *phi)
{
	bool	died;

	if (phi->id % 2)
		died = _take_forks(*phi, phi->right_fork, phi->left_fork);
	else
		died = _take_forks(*phi, phi->left_fork, phi->right_fork);
	if (died)
		return (died);
	pthread_mutex_lock(&phi->meal_mutex);
	phi->last_meal_time = date_now();
	phi->meals_eaten++;
	pthread_mutex_unlock(&phi->meal_mutex);
	died = philog(*phi, ACT_EAT);
	if (died)
		return (died);
	ft_usleep(phi->data->time_to_eat);
	pthread_mutex_unlock(phi->left_fork);
	pthread_mutex_unlock(phi->right_fork);
	return (0);
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
	return (philog(*phi, ACT_THINK));
}

void	*philo_life(void *param)
{
	t_philo	*phi;

	phi = (t_philo *)param;
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
