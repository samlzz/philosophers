/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 14:29:43 by sliziard          #+#    #+#             */
/*   Updated: 2025/03/07 17:59:12 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <pthread.h>

static bool	_check_death(t_philo phi)
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
	ft_usleep(data.time_to_eat);
	if (++phi->meals >= (unsigned int)data.must_eat_count)
		sem_post(data.sem_meals_finished);
	phi->lst_meal_time = date_now();
	sem_post(data.forks);
	sem_post(data.forks);
	return (0);
}

static bool	_sleep(t_philo phi)
{
	if (_check_death(phi))
		return (1);
	philog(phi, ACT_SLEEP);	
	ft_usleep(phi.__dptr->time_to_sleep);
	return (0);
}

static bool	_think(t_philo phi)
{
	if (_check_death(phi))
		return (1);
	philog(phi, ACT_THINK);
	usleep(50);
	return (0);
}

int	children_process(unsigned int id, t_data data)
{
	t_philo		philo;

	philo = (t_philo){id, 0, data.start_time, &data};
	if (id % 2)
		usleep(ODD_DELAY);
	while (1)
	{
		if (_eat(data, &philo) || _sleep(philo) || _think(philo))
			break ;
	}
	sem_close(data.sem_end);
	sem_close(data.sem_print);
	sem_close(data.forks);
	sem_close(data.sem_meals_finished);
	return (0);
}
