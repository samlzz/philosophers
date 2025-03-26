/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 15:32:18 by sliziard          #+#    #+#             */
/*   Updated: 2025/03/26 14:41:55 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>

static inline bool	_take_forks(t_philo *phi)
{
	pthread_mutex_lock(phi->right_fork);
	if (phi->right_fork == phi->left_fork)
	{
		philog(*phi, ACT_FORK, MX_BOTH);
		ft_usleep(phi->next_meal_time - date_now());
		pthread_mutex_unlock(phi->right_fork);
		return (1);
	}
	pthread_mutex_lock(phi->left_fork);
	if (date_now() >= phi->next_meal_time)
	{
		pthread_mutex_unlock(phi->right_fork);
		pthread_mutex_unlock(phi->left_fork);
		return (1);	
	}
	philog(*phi, ACT_FORK, MX_LOCK);
	philog(*phi, ACT_FORK, MX_NONE);
	philog(*phi, ACT_EAT, MX_UNLOCK);
	return (0);
}

static bool	_eat(t_philo *phi)
{
	uint64_t	now;
	uint64_t	next_die;

	if (_take_forks(phi))
		return (1);
	phi->meals_eaten++;
	if (phi->data->must_eat_count != -1 && \
		phi->meals_eaten == phi->data->must_eat_count)
	{
		set_shared(&phi->data->sated, SH_INCREMENT, 1);
	}
	now = date_now() - phi->data->start_time;
	next_die = phi->next_meal_time - phi->data->start_time;
	if (now + phi->data->time_to_eat >= next_die)
		ft_usleep(next_die - now);
	else
		ft_usleep(phi->data->time_to_eat);
	pthread_mutex_unlock(phi->left_fork);
	pthread_mutex_unlock(phi->right_fork);
	if (now + phi->data->time_to_eat >= next_die)
		return (1);
	phi->last_meal_time = date_now();
	phi->next_meal_time = phi->last_meal_time + phi->data->time_to_die;
	return (0);
}

static bool	_sleep(t_philo *phi)
{
	uint64_t	curr_time;

	curr_time = date_now();
	if (!get_shared(&phi->data->sim_state))
		return (0);
	philog(*phi, ACT_SLEEP, MX_BOTH);
	if (curr_time + phi->data->time_to_sleep >= phi->next_meal_time)
	{
		ft_usleep(phi->next_meal_time - date_now());
		return (1);
	}
	ft_usleep(phi->data->time_to_sleep);
	return (0);
}

static bool	_think(t_philo *phi)
{
	if (get_shared(&phi->data->sim_state))
		philog(*phi, ACT_THINK, MX_BOTH);
	if (phi->data->count % 2)
		ft_usleep((phi->next_meal_time - date_now()) / 3);
	return (0);
}

void	*philo_life(void *param)
{
	t_philo	*phi;

	phi = (t_philo *)param;
	while (!get_shared(&phi->data->sim_state))
		ft_usleep(1);
	if (get_shared(&phi->data->sim_state) == -1)
		return (NULL);
	if (phi->id % 2 == 0)
		ft_usleep(3);
	else if ((phi->data->count / 2) % 2 == 0)
		ft_usleep(1);
	phi->last_meal_time = phi->data->start_time;
	phi->next_meal_time = phi->last_meal_time + phi->data->time_to_die;
	while (get_shared(&phi->data->sim_state))
	{
		if (_eat(phi) || _sleep(phi) || _think(phi))
		{
			if (get_shared(&phi->data->sim_state))
				(philog(*phi, ACT_DIE, MX_BOTH), \
					set_shared(&phi->data->sim_state, SH_SET, false));
			break ;
		}
	}
	return (NULL);
}
