/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 14:29:43 by sliziard          #+#    #+#             */
/*   Updated: 2025/03/06 17:08:55 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static bool	_check_death(t_philo phi, long time_to_die)
{
	if (date_now() - phi.lst_meal_time >= time_to_die)
	{
		philog(phi, ACT_DIE);
		sem_post(phi.sem_end);
		return (1);
	}
	return (0);
}

static bool	_eat(t_data data, t_philo *phi)
{
	sem_wait(phi->forks);
	philog(*phi, ACT_FORK);
	if (_check_death(*phi, data.time_to_die))
		return (1);
	sem_wait(phi->forks);
	philog(*phi, ACT_FORK);
	if (_check_death(*phi, data.time_to_die))
		return (1);
	philog(*phi, ACT_EAT);
	ft_usleep(data.time_to_eat);
	phi->meals++;
	phi->lst_meal_time = date_now();
	sem_post(phi->forks);
	sem_post(phi->forks);
	if (data.must_eat_count == -1)
		return (0);
	return (phi->meals >= (unsigned int)data.must_eat_count);
}

static bool	_sleep(t_data data, t_philo phi)
{
	if (_check_death(phi, data.time_to_die))
		return (1);
	philog(phi, ACT_SLEEP);	
	ft_usleep(data.time_to_sleep);
	return (0);
}

static bool	_think(t_data data, t_philo phi)
{
	if (_check_death(phi, data.time_to_die))
		return (1);
	philog(phi, ACT_THINK);
	usleep(50);
	return (0);
}

int	children_process(unsigned int id, t_data data)
{
	t_philo	phi;

	phi = (t_philo){id, 0, data.start_time, NULL, NULL, NULL};
	if (open_semaphores(&phi, data.count))
		return (1);
	if (phi.id % 2)
		usleep(200);
	while (1)
	{
		if (_eat(data, &phi) || _sleep(data, phi) || _think(data, phi))
			break ;
	}
	sem_close(phi.sem_print);
	sem_close(phi.sem_end);
	sem_close(phi.sem_print);
	return (0);
}
