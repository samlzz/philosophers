/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 15:32:18 by sliziard          #+#    #+#             */
/*   Updated: 2025/03/10 19:57:08 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>

static inline bool	_take_forks(t_philo *phi)
{
	pthread_mutex_lock(phi->left_fork);
	philog(*phi, ACT_FORK);
	if (phi->left_fork == phi->right_fork || \
		date_now() >= phi->next_meal_time)
	{
		pthread_mutex_unlock(phi->left_fork);
		return (1);
	}
	pthread_mutex_lock(phi->right_fork);
	philog(*phi, ACT_FORK);
	if (date_now() >= phi->next_meal_time)
	{
		pthread_mutex_unlock(phi->left_fork);
		pthread_mutex_unlock(phi->right_fork);
		return (1);
	}
	return (0);
}

static bool	_eat(t_philo *phi)
{
	bool	exit;

	if (_take_forks(phi))
		return (1);
	phi->last_meal_time = date_now();
	phi->meals_eaten++;
	if (phi->data->must_eat_count != -1 && \
		phi->meals_eaten == phi->data->must_eat_count)
	{
		set_shared(&phi->data->sated, SH_INCREMENT, 1);
	}
	philog(*phi, ACT_EAT);
	if (phi->last_meal_time + phi->data->time_to_eat >= phi->next_meal_time)
		ft_usleep(phi->next_meal_time - phi->last_meal_time);
	else
		ft_usleep(phi->data->time_to_eat);
	pthread_mutex_unlock(phi->left_fork);
	pthread_mutex_unlock(phi->right_fork);
	exit = date_now() >= phi->next_meal_time;
	phi->next_meal_time = phi->last_meal_time + phi->data->time_to_die;
	return (exit);
}

static bool	_sleep(t_philo *phi)
{
	uint64_t	curr_time;

	curr_time = date_now();
	if (!get_shared(phi->data->sim_state))
		return (0);
	philog(*phi, ACT_SLEEP);
	if (curr_time + phi->data->time_to_sleep >= phi->next_meal_time)
	{
		if (curr_time > phi->next_meal_time)
			ft_usleep(phi->next_meal_time - curr_time);
		return (1);
	}
	ft_usleep(phi->data->time_to_sleep);
	return (0);
}

static bool	_think(t_philo *phi)
{
	if (get_shared(phi->data->sim_state))
		philog(*phi, ACT_THINK);
	if (phi->data->count % 2)
		ft_usleep((phi->next_meal_time - date_now()) / 3);
	return (0);
}

void	*philo_life(void *param)
{
	t_philo	*phi;

	phi = (t_philo *)param;
	while (!get_shared(phi->data->sim_state))
		ft_usleep(1);
	if (phi->id % 2 == 0)
		ft_usleep(3);
	else if ((phi->data->count / 2) % 2 == 0)
		ft_usleep(1);
	phi->last_meal_time = date_now();
	phi->next_meal_time = phi->last_meal_time + phi->data->time_to_die;
	while (get_shared(phi->data->sim_state))
	{
		if (_eat(phi) || _sleep(phi) || _think(phi))
		{
			philog(*phi, ACT_DIE);
			set_shared(&phi->data->sim_state, SH_SET, false);
			break ;
		}
	}
	return (NULL);
}
