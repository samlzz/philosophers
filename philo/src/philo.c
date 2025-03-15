/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:08:29 by sliziard          #+#    #+#             */
/*   Updated: 2025/03/15 15:26:19 by sliziard         ###   ########.fr       */
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

static inline void	_destroy_data(t_data *d_ptr)
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
	if (IS_ALLOC)
	{
		free(d_ptr->philos);
		free(d_ptr->forks);
	}
}

static inline int16_t	_launch_sim(t_data *dptr)
{
	size_t		i;
	pthread_t	monitor;
	int16_t		exit;

	if (dptr->must_eat_count == -1)
		return (0);
	exit = 0;
	if (pthread_create(&monitor, NULL, &meal_monitor, dptr))
	{
		set_shared(&dptr->sim_state, SH_SET, false);
		pthread_join(monitor, NULL);
		exit = 1;
	}
	else
		pthread_detach(&monitor);
	i = 0;
	while (i < dptr->count)
		pthread_join(dptr->philos[i++].thread, NULL);
	return (exit);
}

#ifndef ALLOC_MODE

int	main(int argc, char const *argv[])
{
	t_data		data;
	t_philo		philos[PHI_MAX + 1];
	t_mutex		forks[PHI_MAX + 1];
	int16_t		exit_code;

	if (argc < 5 || argc > 6)
		return (write(2, ERR_ARG_NB, 160), 1);
	exit_code = init_data(&data, argc - 1, argv + 1);
	if (exit_code == 2)
		return (write(2, ERR_PHI_MAX_TOHIGH, 66), 1);
	else if (exit_code)
		return (write(2, ERR_INVALID_ARG, 104), 1);
	if (init_philo_and_forks(&data, philos, forks))
		return (1);
	exit_code = _launch_sim(&data);
	_destroy_data(&data);
	return (exit_code);
}

#else

int	main(int argc, char const *argv[])
{
	t_data		data;
	t_philo		*philos;
	t_mutex		*forks;
	int16_t		exit_code;

	if (argc < 5 || argc > 6)
		return (write(2, ERR_ARG_NB, 160), 1);
	if (init_data(&data, argc - 1, argv + 1))
		return (write(2, ERR_INVALID_ARG, 104), 1);
	philos = malloc(data.count * sizeof(t_philo));
	forks = malloc(data.count * sizeof(t_mutex));
	if (!philos || !forks)
		return (free(philos), free(forks), 1);
	if (init_philo_and_forks(&data, philos, forks))
		return (free(philos), free(forks), 1);
	exit_code = _launch_sim(&data);
	return (_destroy_data(&data), exit_code);
}

#endif
