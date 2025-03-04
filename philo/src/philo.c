/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:08:29 by sliziard          #+#    #+#             */
/*   Updated: 2025/03/02 20:01:07 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

static void	_check_end(t_data *data)
{
	size_t	i;
	bool	have_all_eat;

	have_all_eat = true;
	i = 0;
	while (i < data->count)
	{
		if (data->must_eat_count != -1 && \
			data->philos[i].meals_eaten < data->must_eat_count)
			have_all_eat = false;
		if (date_now() - data->philos[i].last_meal_time >= data->time_to_die)
		{
			philog(data->philos[i], ACT_DIE);
			data->sim_end = true;
			break ;
		}
		i++;
	}
	data->sim_end = have_all_eat;
}

void	*monitoring(void *param)
{
	t_data	*data;

	data = (t_data *)param;
	if (data->count < 2)
	{
		if (data->philos)
			philog(*data->philos, ACT_DIE);
		data->sim_end = true;
	}
	while (!data->sim_end)
	{
		_check_end(data);
		usleep(MONITOR_DELAY);
	}
	return (NULL);
}

static void	_destroy_data(t_data *d_ptr)
{
	size_t	i;

	free(d_ptr->philos);
	i = 0;
	while (i < d_ptr->count)
		pthread_mutex_destroy(d_ptr->forks + i++);
	free(d_ptr->forks);
	pthread_mutex_destroy(&d_ptr->print_mutex);
}

int main(int argc, char const *argv[])
{
	t_data		data;
	pthread_t	monitor;
	size_t		i;

	if (argc < 5 || argc > 6)
		return (write(2, ERR_ARG_NB, 160), 1);
	if (init_data(&data, argc - 1, argv + 1))
		return (write(2, ERR_INVALID_ARG, 126), 1);
	pthread_create(&monitor, NULL, &monitoring, (void *)&data);
	pthread_join(monitor, NULL);
	i = 0;
	while (i < data.count)
		pthread_join(data.philos[i++].thread, NULL);
	_destroy_data(&data);
	return (0);
}
