/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 12:46:40 by sliziard          #+#    #+#             */
/*   Updated: 2025/03/11 17:39:41 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <pthread.h>	
#include <stdio.h>

static void	_wait_childrens(pid_t *childs, size_t count)
{
	size_t	i;

	i = 0;
	while (i < count)
		kill(childs[i++], SIGKILL);
	i = 0;
	while (i < count)
		waitpid(childs[i++], NULL, 0);
	free(childs);
}

void	*meal_monitor(void *param)
{
	t_data	*data;
	size_t	i;

	data = (t_data *)param;
	i = 0;
	while (i < data->count)
	{
		sem_post(data->sem_start);
		i++;
	}
	data->start_time = date_now();
	i = 0;
	while (i < data->count)
	{
		sem_wait(data->sem_meals_finished);
		i++;
	}
	usleep(MONITOR_DELAY);
	printf("%ld All philosophers have eaten enough.\n", \
		date_now() - data->start_time);
	sem_post(data->sem_end);
	return (NULL);
}

int	main(int argc, char const *argv[])
{
	t_data		data;
	pid_t		*childs;
	pthread_t	monitor;

	if (argc < 5 || argc > 6)
		return (write(2, ERR_ARG_NB, 160), 1);
	if (init_data(&data, argc - 1, argv + 1))
	{
		write(2, ERR_INVALID_ARG, 104);
		return (1);
	}
	childs = init_childs(&data);
	if (!childs)
		return (destroy_data(NULL), 1);
	if (data.must_eat_count != -1)
		(pthread_create(&monitor, NULL, &meal_monitor, &data), \
			pthread_detach(monitor));
	sem_wait(data.sem_end);
	_wait_childrens(childs, data.count);
	close_sems(&data);
	return (0);	
}
