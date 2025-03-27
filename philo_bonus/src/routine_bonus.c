/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 14:29:43 by sliziard          #+#    #+#             */
/*   Updated: 2025/03/27 18:47:54 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static inline bool	_take_forks(t_philo phi, t_data data)
{
	sem_wait(data.forks);
	if (data.count == 1)
		return (philog(phi, ACT_FORK, false), \
				ft_usleep(phi.next_meal_time - date_now()), \
				sem_post(data.forks), 1);
	sem_wait(data.forks);
	sem_wait(data.sem_print);
	if (date_now() >= phi.next_meal_time)
		return (sem_post(data.forks), \
				sem_post(data.forks), \
				sem_post(data.sem_print), 1);
	philog(phi, ACT_FORK, true);
	philog(phi, ACT_FORK, true);
	philog(phi, ACT_EAT, true);
	sem_post(data.sem_print);
	return (0);
}

static bool	_eat(t_data data, t_philo *phi)
{
	uint64_t	now;
	uint64_t	next_die;

	if (_take_forks(*phi, data))
		return (1);
	now = date_now() - data.start_time;
	next_die = phi->next_meal_time - data.start_time;
	if (now + data.time_to_eat >= next_die)
		ft_usleep(next_die - now);
	else
		ft_usleep(data.time_to_eat);
	sem_post(data.forks);
	sem_post(data.forks);
	if (now + data.time_to_eat >= next_die)
		return (1);
	phi->lst_meal_time = date_now();
	phi->next_meal_time = phi->lst_meal_time + data.time_to_die;
	phi->meals++;
	if (data.must_eat_count != -1 && \
		phi->meals == (uint32_t)data.must_eat_count)
		sem_post(data.sem_sated);
	return (0);
}

static bool	_sleep(t_philo phi, t_data data)
{
	int64_t	curr_time;

	philog(phi, ACT_SLEEP, false);
	curr_time = date_now();
	if (curr_time + data.time_to_sleep >= phi.next_meal_time)
	{
		ft_usleep(phi.next_meal_time - curr_time);
		return (1);
	}
	ft_usleep(data.time_to_sleep);
	return (0);
}

static bool	_think(t_philo phi, t_data data)
{
	philog(phi, ACT_THINK, false);
	if (data.count % 2)
		ft_usleep((phi.next_meal_time - date_now()) / 3);
	return (0);
}

int16_t	children_process(uint32_t id, t_data data)
{
	t_philo		philo;

	sem_wait(data.sem_start);
	data.start_time = date_now();
	philo = (t_philo){id, 0, data.start_time, \
		data.start_time + data.time_to_die, &data};
	if (id % 2 == 0)
		ft_usleep(3);
	while (1)
	{
		if (_eat(data, &philo) || \
			_sleep(philo, data) || \
			_think(philo, data))
		{
			sem_post(data.sem_end);
			philog(philo, ACT_DIE, false);
			break ;
		}
	}
	close_sems(&data);
	return (0);
}
