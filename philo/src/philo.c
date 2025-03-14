/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:08:29 by sliziard          #+#    #+#             */
/*   Updated: 2025/03/14 14:30:32 by sliziard         ###   ########.fr       */
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
	while (get_shared(&data->sim_state))
	{
		if ((uint32_t)get_shared(&data->sated) == data->count)
		{
			set_shared(&data->sim_state, SH_SET, false);
			killed = false;
			break ;
		}
		ft_usleep(MONITOR_DELAY);
	}
	if (!killed)
		printf("%ld All philosophers have eaten enough.\n", \
			date_now() - data->start_time);
	return (NULL);
}

static inline void	_destroy_data(t_data *d_ptr, bool alloc_mode)
{
	size_t	i;

	if (d_ptr->forks)
	{
		i = 0;
		while (i < d_ptr->count)
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
	t_philo		philos[PHI_MAX + 1];
	t_mutex		forks[PHI_MAX + 1];
	pthread_t	monitor;
	size_t		i;

	if (argc < 5 || argc > 6)
		return (write(2, ERR_ARG_NB, 160), 1);
	i = init_data(&data, argc - 1, argv + 1);
	if (i == 2)
		return (write(2, ERR_PHI_MAX_TOHIGH, 66), 1);
	else if (i)
		return (write(2, ERR_INVALID_ARG, 104), 1);
	init_philo_and_forks(&data, philos, forks);
	if (data.must_eat_count != -1)
	{
		pthread_create(&monitor, NULL, &meal_monitor, &data);
		pthread_detach(monitor);
	}
	i = 0;
	while (i < data.count)
		pthread_join(data.philos[i++].thread, NULL);
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
	size_t		i;

	if (argc < 5 || argc > 6)
		return (write(2, ERR_ARG_NB, 160), 1);
	i = init_data(&data, argc - 1, argv + 1);
	if (i == 2)
		return (write(2, ERR_PHI_MAX_TOHIGH, 66), 1);
	else if (i)
		return (write(2, ERR_INVALID_ARG, 104), 1);
	(data.philos = (philos = malloc(data.count * sizeof(t_philo))));
	forks = malloc(data.count * sizeof(t_mutex));
	if (!philos || !forks)
		return (_destroy_data(&data, true) 1);
	init_philo_and_forks(&data, philos, forks);
	if (data.must_eat_count != -1)
		(pthread_create(&monitor, NULL, &monitoring, (void *)&data), \
			pthread_join(monitor, NULL));
	i = 0;
	while (i < data.count)
		pthread_join(data.philos[i++].thread, NULL);
	return (_destroy_data(&data, true), 0);
}

#endif
