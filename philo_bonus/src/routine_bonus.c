/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 14:29:43 by sliziard          #+#    #+#             */
/*   Updated: 2025/03/16 22:45:30 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static inline bool	_take_forks(t_philo phi, t_data data)
{
	sem_wait(data.forks);
	philog(phi, ACT_FORK);
	if (date_now() >= phi.next_meal_time)
	{
		sem_post(data.forks);
		return (1);
	}
	sem_wait(data.forks);
	philog(phi, ACT_FORK);
	if (date_now() >= phi.next_meal_time)
	{
		sem_post(data.forks);
		sem_post(data.forks);
		return (1);
	}
	return (0);
}

static bool	_eat(t_data data, t_philo *phi)
{
	if (_take_forks(*phi, data))
		return (1);
	phi->meals++;
	if (data.must_eat_count != -1 && phi->meals == (uint32_t)data.must_eat_count)
		sem_post(data.sem_sated);
	phi->lst_meal_time = date_now();
	phi->next_meal_time = phi->lst_meal_time + data.time_to_die;
	philog(*phi, ACT_EAT);
	if (phi->lst_meal_time + data.time_to_eat >= phi->next_meal_time)
		ft_usleep(phi->next_meal_time - phi->lst_meal_time);
	else
		ft_usleep(data.time_to_eat);
	sem_post(data.forks);
	sem_post(data.forks);
	return (date_now() >= phi->next_meal_time);
}

static bool	_sleep(t_philo phi, t_data data)
{
	int64_t	curr_time;

	curr_time = date_now();
	philog(phi, ACT_SLEEP);
	if (curr_time + data.time_to_sleep >= phi.next_meal_time)
	{
		if (curr_time > phi.next_meal_time)
			ft_usleep(phi.next_meal_time - curr_time);
		return (1);
	}
	ft_usleep(data.time_to_sleep);
	return (0);
}

static bool	_think(t_philo phi, t_data data)
{
	philog(phi, ACT_THINK);
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
	else if ((data.count / 2) % 2 == 0)
		ft_usleep(1);
	while (1)
	{
		if (_eat(data, &philo) || \
			_sleep(philo, data) || \
			_think(philo, data))
		{
			philog(philo, ACT_DIE);
			sem_post(data.sem_end);
			break ;
		}
	}
	close_sems(&data);
	return (0);
}
