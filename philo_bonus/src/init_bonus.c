/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 12:56:14 by sliziard          #+#    #+#             */
/*   Updated: 2025/03/16 22:43:06 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <signal.h>
#include <fcntl.h>

static inline uint32_t	_ascii_to_uint(const char *nptr, int *error)
{
	uint64_t	r;

	r = 0;
	*error = 0;
	if (!nptr || !*nptr)
	{
		*error = 1;
		return (0);
	}
	while (*nptr >= '0' && *nptr <= '9')
	{
		r = (r * 10) + (*nptr - '0');
		if (r > UINT_MAX)
		{
			*error = 1;
			return (UINT_MAX);
		}
		nptr++;
	}
	if (*nptr)
		*error = 1;
	return ((uint32_t)r);
}

static int16_t	_set_nb_field(char const *nb, int *sign_field, uint32_t *field)
{
	int16_t		has_err;
	uint32_t	value;

	has_err = 0;
	value = ascii_to_uint(nb, &has_err);
	if (sign_field)
	{
		if (value > INT_MAX || !value)
			return (1);
		*sign_field = (int)value;
	}
	else
		*field = value;
	return (has_err);
}

int16_t	init_data(t_data *d_ptr, int32_t ac, char const *av[])
{
	memset(d_ptr, 0, sizeof(t_data));
	if (_set_nb_field(av[0], NULL, &d_ptr->count))
		return (1);
	if (_set_nb_field(av[1], NULL, &d_ptr->time_to_die))
		return (1);
	if (_set_nb_field(av[2], NULL, &d_ptr->time_to_eat))
		return (1);
	if (_set_nb_field(av[3], NULL, &d_ptr->time_to_sleep))
		return (1);
	if (ac == 5 && _set_nb_field(av[4], &d_ptr->must_eat_count, NULL))
		return (1);
	if (ac == 4)
		d_ptr->must_eat_count = -1;
	return (0);
}

static inline int16_t	open_semaphores(t_data *d_ptr)
{
	d_ptr->sem_end = sem_open(SEM_END, O_CREAT, 0644, 0);
	if (d_ptr->sem_end == SEM_FAILED)
		return (1);
	d_ptr->sem_start = sem_open(SEM_START, O_CREAT, 0644, 0);
	if (d_ptr->sem_start == SEM_FAILED)
		return (sem_close(d_ptr->sem_end), 1);
	d_ptr->sem_print = sem_open(SEM_PRINT, O_CREAT, 0644, 1);
	if (d_ptr->sem_print == SEM_FAILED)
		return (close_sems(d_ptr), 1);
	if (d_ptr->must_eat_count != -1)
	{
		d_ptr->sem_sated = sem_open(SEM_MEALS, O_CREAT, 0644, 0);
		if (d_ptr->sem_sated == SEM_FAILED)
			return(close_sems(d_ptr), 1);
	}
	d_ptr->forks = sem_open(SEM_FORKS, O_CREAT, 0644, d_ptr->count);
	if (d_ptr->forks == SEM_FAILED)
		return(close_sems(d_ptr), 1);
	sem_unlink(SEM_END);
	sem_unlink(SEM_START);
	sem_unlink(SEM_PRINT);
	sem_unlink(SEM_FORKS);
	if (d_ptr->sem_sated)
		sem_unlink(SEM_MEALS);
	return (0);
}

pid_t	*init_childs(t_data *data)
{
	size_t	i;
	pid_t	*childs;

	childs = malloc(sizeof(pid_t) * data->count);
	if (!childs)
		return (NULL);
	if (open_semaphores(data))
		return (free(childs), NULL);
	i = 0;
	while (i < data->count)
	{
		childs[i] = fork();
		if (!childs[i])
			exit(children_process(i + 1, *data));
		else if (childs[i] == -1)
		{
			while (i > 0)
				kill(childs[--i], SIGKILL);
			close_sems(data);
			return (free(childs), NULL);
		}
		i++;
	}
	return (childs);
}
