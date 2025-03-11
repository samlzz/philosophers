/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 14:29:43 by sliziard          #+#    #+#             */
/*   Updated: 2025/03/11 17:33:16 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <pthread.h>

static inline bool	_check_death(t_philo phi)
{
	if (date_now() - phi.lst_meal_time >= phi.__dptr->time_to_die)
	{
		sem_post(phi.__dptr->sem_end);
		philog(phi, ACT_DIE);
		return (1);
	}
	return (0);
}

static bool	_eat(t_data data, t_philo *phi)
{
	sem_wait(data.forks);
	philog(*phi, ACT_FORK);
	if (_check_death(*phi))
		return (1);
	sem_wait(data.forks);
	philog(*phi, ACT_FORK);
	if (_check_death(*phi))
		return (1);
	philog(*phi, ACT_EAT);
	phi->lst_meal_time = date_now();
	phi->next_meal_time = phi->lst_meal_time + data.time_to_die;
	if (phi->lst_meal_time + data.time_to_eat >= phi->next_meal_time)
		ft_usleep(phi->next_meal_time - phi->lst_meal_time);
	else
		ft_usleep(data.time_to_eat);
	if (++phi->meals == (unsigned int)data.must_eat_count)
		sem_post(data.sem_meals_finished);
	sem_post(data.forks);
	sem_post(data.forks);
	return (0);
}

static bool	_sleep(t_philo phi, t_data data)
{
	long	curr_time;

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
	if (_check_death(phi))
		return (1);
	philog(phi, ACT_THINK);
	if (data.count % 2)
		ft_usleep((phi.next_meal_time - date_now()) / 3);
	return (0);
}

int	children_process(unsigned int id, t_data data)
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
			break ;
		}
	}
	sem_close(data.sem_end);
	sem_close(data.sem_print);
	sem_close(data.forks);
	sem_close(data.sem_meals_finished);
	return (0);
}
