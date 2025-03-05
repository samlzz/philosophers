/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:08:29 by sliziard          #+#    #+#             */
/*   Updated: 2025/03/05 11:39:59 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>

#ifdef ALLOC_MODE
# include <stdlib.h>
#endif

static void	_check_end(t_data *data)
{
	size_t	i;
	bool	have_all_eat;
	long	lst_meal;

	i = 0;
	have_all_eat = data->must_eat_count != -1;
	while (i < data->count)
	{
		pthread_mutex_lock(&data->philos[i].meal_mutex);
		if (data->philos[i].meals_eaten < data->must_eat_count)
			have_all_eat = false;
		lst_meal = data->philos[i].last_meal_time;
		pthread_mutex_unlock(&data->philos[i].meal_mutex);
		if (date_now() - lst_meal >= data->time_to_die)
		{
			set_sim_end(data, true);
			philog(data->philos[i], ACT_DIE);
			return ;
		}
		i++;
	}
	if (have_all_eat)
		set_sim_end(data, true);
}

void	*monitoring(void *param)
{
	t_data	*data;

	data = (t_data *)param;
	if (data->count < 2)
	{
		set_sim_end(data, true);
		if (data->philos)
			philog(*data->philos, ACT_DIE);
	}
	while (!get_sim_end(data))
	{
		_check_end(data);
		ft_usleep(MONITOR_DELAY);
	}
	return (NULL);
}

static void	_destroy_data(t_data *d_ptr)
{
	size_t	i;

	i = 0;
	while (i < d_ptr->count)
	{
		pthread_mutex_destroy(&d_ptr->philos[i].meal_mutex);
		pthread_mutex_destroy(d_ptr->forks + i++);
	}
	pthread_mutex_destroy(&d_ptr->print_mutex);
	pthread_mutex_destroy(&d_ptr->end_mutex);
	#ifdef ALLOC_MODE

	free(d_ptr->philos);
	free(d_ptr->forks);
	#endif

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
		return (write(2, ERR_INVALID_ARG, 126), 1);
	init_philo_and_forks(&data, philos, forks);
	pthread_create(&monitor, NULL, &monitoring, (void *)&data);
	pthread_join(monitor, NULL);
	while (data.count)
		pthread_join(data.philos[--data.count].thread, NULL);
	_destroy_data(&data);
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
		return (write(2, ERR_INVALID_ARG, 126), 1);
	philos = malloc(count * sizeof(t_philo));
	if (!philos)
		return (write(2, ERR_ALLOC, 25), 1);
	forks = malloc(count * sizeof(t_mutex));
	if (!fork)
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
	return (_destroy_data(&data), 0);
}

#endif
