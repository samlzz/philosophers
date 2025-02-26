/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:47:49 by sliziard          #+#    #+#             */
/*   Updated: 2025/02/26 15:16:10 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <limits.h>
#include <stdlib.h>
#include <string.h>

int	ft_satoi(char const *nptr, int *error)
{
	long	r;
	int		s;

	r = 0;
	s = 1;
	*error = 0;
	if (*nptr == '-' || *nptr == '+')
	{
		if (*nptr == '-')
			s = -1;
		nptr++;
	}
	while (*nptr >= '0' && *nptr <= '9' && *nptr)
	{
		r = (r * 10) + *nptr - '0';
		if ((s == 1 && r > INT_MAX) || (s == -1 && (r * -1) < INT_MIN))
		{
			*error = 1;
			return ((s == 1) * INT_MAX + (s != 1) * INT_MIN);
		}
		nptr++;
	}
	if (*nptr || *(nptr - 1) == '-' || *(nptr - 1) == '+')
		*error = 1;
	return ((int)r * s);
}

pthread_mutex_t	*init_forks(size_t count)
{
	size_t			i;
	pthread_mutex_t	*forks;

	forks = malloc(sizeof(pthread_mutex_t) * count);
	if (!forks)
		return (NULL);
	i = 0;
	while (i < count)
		pthread_mutex_init(forks + i++, NULL);
	return (forks);
}

void	init_philos(t_data *d_ptr)
{
	int		i;

	memset(d_ptr->philos, 0, sizeof(t_philo) * d_ptr->count);
	i = 0;
	while (i < d_ptr->count)
	{
		d_ptr->philos[i].id = i + 1;
		d_ptr->philos[i].last_meal_time = d_ptr->start_time;
		d_ptr->philos[i].left_fork = d_ptr->forks + i;
		d_ptr->philos[i].right_fork = d_ptr->forks + ((i + 1) % d_ptr->count);
		d_ptr->philos[i].data = d_ptr;
		i++;
	}
}

static int	_set_nb_field(int *field, char const *nb)
{
	int	has_err;

	has_err = 0;
	*field = ft_satoi(nb, &has_err);
	return (has_err);
}

int	init_data(t_data *d_ptr, int ac, char const *av[])
{
	memset(d_ptr, 0, sizeof(t_data));
	d_ptr->start_time = date_now();
	if (_set_nb_field(&d_ptr->count, av[0]))
		return (1);
	if (_set_nb_field(&d_ptr->time_to_die, av[1]))
		return (1);
	if (_set_nb_field(&d_ptr->time_to_eat, av[2]))
		return (1);
	if (_set_nb_field(&d_ptr->time_to_sleep, av[3]))
		return (1);
	if (ac == 5)
	{
		if (_set_nb_field(&d_ptr->must_eat_count, av[4]))
			return (1);
	}
	else
		d_ptr->must_eat_count = -1;
	d_ptr->philos = malloc(sizeof(t_philo) * d_ptr->count);
	if (!d_ptr->philos)
		return (1);
	d_ptr->forks = init_forks(d_ptr->count);
	if (!d_ptr->forks)
		return (free(d_ptr->philos), 1);
	pthread_mutex_init(&d_ptr->print_mutex, NULL);
	return (init_philos(d_ptr), 0);
}
