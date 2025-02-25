/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:47:49 by sliziard          #+#    #+#             */
/*   Updated: 2025/02/25 20:19:08 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <limits.h>
#include <stdlib.h>

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

static bool	_set_nb_field(int *field, char const *nb)
{
	bool	has_err;

	has_err = false;
	*field = ft_satoi(nb, &has_err);
	return (has_err);
}

pthread_mutex_t	*init_forks(size_t count)
{
	size_t			i;
	pthread_mutex_t	forks;

	forks = malloc(sizeof(pthread_mutex_t) * count);
	if (!forks)
		return (NULL);
	i = 0;
	while (i < count)
		pthread_mutex_init(forks + i++, NULL);
	return (forks);
}

int	init_data(t_data *d_ptr, int ac, char const *av)
{

	d_ptr->start_time = date_now();
	if (set_nb_field(&d_ptr->nb_philos, av[0]))
		return (1);
	if (set_nb_field(&d_ptr->time_to_die, av[1]))
		return (1);
	if (set_nb_field(&d_ptr->time_to_eat, av[2]))
		return (1);
	if (set_nb_field(&d_ptr->time_to_sleep, av[3]))
		return (1);
	if (ac == 5)
		if (set_nb_field(&d_ptr->must_eat_count, av[4]))
			return (1);
	else
		d_ptr->must_eat_count = -1;
	d_ptr->sim_end = 1;
	d_ptr->philos = malloc(sizeof(t_philo) * d_ptr->nb_philos);
	if (!d_ptr->philos)
		return (1);
	d_ptr->forks = init_forks(d_ptr->nb_philos);
	if (!d_ptr->forks)
		return (free(d_ptr->philos), 1);
	pthread_mutex_init(&d_ptr->print_mutex, NULL);
	return (0);
}
