/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:08:29 by sliziard          #+#    #+#             */
/*   Updated: 2025/03/10 18:55:48 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

static void	*meal_monitor(void *param)
{
	t_data	*data;
	bool	killed;

	killed = true;
	data = (t_data *)param;
	while (get_shared(data->sim_state))
	{
		if ((uint32_t)get_shared(data->sated) == data->count)
		{
			set_shared(&data->sim_state, SH_SET, false);
			killed = false;
			break ;
		}
		ft_usleep(MONITOR_DELAY);
	}
	if (!killed)
		printf("[%ld] All philosophers have eaten enough.\n", \
			date_now() - data->start_time);
	return (NULL);
}

static inline void	_destroy_data(t_data *d_ptr, bool alloc_mode)
{
	size_t	i;

	i = 0;
	while (i < d_ptr->count)
	{
		pthread_mutex_destroy(d_ptr->forks + i++);
	}
	pthread_mutex_destroy(&d_ptr->print_mutex);
	pthread_mutex_destroy(&d_ptr->sim_state.mtx);
	pthread_mutex_destroy(&d_ptr->sated.mtx);
	if (alloc_mode)
	{
		free(d_ptr->philos);
		free(d_ptr->forks);
	}
}

#ifndef ALLOC_MODE

int	main(int argc, char const *argv[])
{
	t_data		data;
	t_philo		philos[PHILO_MAX];
	t_mutex		forks[PHILO_MAX];
	pthread_t	monitor;

	if (argc < 5 || argc > 6)
		return (write(2, ERR_ARG_NB, 160), 1);
	if (init_data(&data, argc - 1, argv + 1))
		return (write(2, ERR_INVALID_ARG, 104), 1);
	init_philo_and_forks(&data, philos, forks);
	pthread_create(&monitor, NULL, &meal_monitor, &data);
	pthread_detach(monitor);
	while (data.count)
		pthread_join(data.philos[--data.count].thread, NULL);
	_destroy_data(&data, false);
	return (0);
}

#else

int	main(int argc, char const *argv[])
{
	t_data		data;
	t_philo		*philos;
	t_mutex		*forks;
	pthread_t	monitor;

	if (argc < 5 || argc > 6)
		return (write(2, ERR_ARG_NB, 160), 1);
	if (init_data(&data, argc - 1, argv + 1))
		return (write(2, ERR_INVALID_ARG, 104), 1);
	philos = malloc(data.count * sizeof(t_philo));
	if (!philos)
		return (write(2, ERR_ALLOC, 25), 1);
	forks = malloc(data.count * sizeof(t_mutex));
	if (!forks)
	{
		free(philos);
		write(2, ERR_ALLOC, 25);
		return (1);
	}
	init_philo_and_forks(&data, philos, forks);
	pthread_create(&monitor, NULL, &monitoring, (void *)&data);
	pthread_join(monitor, NULL);
	while (data.count)
		pthread_join(data.philos[--data.count].thread, NULL);
	return (_destroy_data(&data, true), 0);
}

#endif
