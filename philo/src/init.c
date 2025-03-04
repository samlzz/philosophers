/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:47:49 by sliziard          #+#    #+#             */
/*   Updated: 2025/03/04 19:31:13 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>

unsigned int ascii_to_uint(const char *nptr, int *error)
{
    unsigned long r;

    r = 0;
    *error = 0;
    if (!nptr || !*nptr)
    {
        *error = 1;
        return 0;
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
    return (unsigned int)r;
}

static int	_set_nb_field(char const *nb, int *signed_field, unsigned int *field)
{
	int				has_err;
	unsigned int	value;

	has_err = 0;
	value = ascii_to_uint(nb, &has_err);
	if (signed_field)
	{
		if (value > INT_MAX)
			return (1);
		*signed_field = (int)value;
	}
	else
		*field = value;
	return (has_err);
}

int	init_data(t_data *d_ptr, int ac, char const *av[])
{
	memset(d_ptr, 0, sizeof(t_data));
	d_ptr->start_time = date_now();
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
	pthread_mutex_init(&d_ptr->print_mutex, NULL);
	pthread_mutex_init(&d_ptr->end_mutex, NULL);
	return (0);
}

static void	_init_philos(t_data *d_ptr, t_philo *philos)
{
	size_t	i;

	i = 0;
	while (i < d_ptr->count)
	{
		philos[i].id = i + 1;
		philos[i].last_meal_time = d_ptr->start_time;
		philos[i].left_fork = d_ptr->forks + i;
		philos[i].right_fork = d_ptr->forks + ((i + 1) % d_ptr->count);
		philos[i].data = d_ptr;
		pthread_mutex_init(&philos[i].meal_mutex, NULL);
		pthread_create(&philos[i].thread, NULL, &philo_life, philos + i);
		i++;
	}
}

void	init_philo_and_forks(t_data *d_ptr, t_philo *philos, t_mutex *forks)
{
	size_t	i;

	i = 0;
	while (i < d_ptr->count)
		pthread_mutex_init(forks + i++, NULL);
	d_ptr->forks = forks;
	memset(philos, 0, sizeof(t_philo) * d_ptr->count);
	_init_philos(d_ptr, philos);
	d_ptr->philos = philos;
}
