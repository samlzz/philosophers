/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 20:54:03 by sliziard          #+#    #+#             */
/*   Updated: 2025/03/10 19:02:35 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <string.h>
#include <limits.h>

static inline uint32_t	ascii_to_uint(const char *nptr, int *error)
{
	unsigned long	r;

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
	return ((unsigned int)r);
}

static inline int	_set_nb_field(char const *nb, int32_t *sign_field, \
	uint32_t *field)
{
	int32_t		has_err;
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

int	init_data(t_data *d_ptr, int ac, char const *av[])
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
	if (pthread_mutex_init(&d_ptr->print_mutex, NULL))
		return (1);
	if (pthread_mutex_init(&d_ptr->sim_state.mtx, NULL))
		return (pthread_mutex_destroy(&d_ptr->print_mutex), 1);
	if (pthread_mutex_init(&d_ptr->sated.mtx, NULL))
	{
		pthread_mutex_destroy(&d_ptr->print_mutex);
		pthread_mutex_destroy(&d_ptr->sim_state.mtx);
		return (1);
	}
	return (0);
}

static inline void	_init_philos(t_data *d_ptr, t_philo *philos)
{
	size_t	i;

	i = 0;
	while (i < d_ptr->count)
	{
		philos[i].id = i + 1;
		philos[i].left_fork = d_ptr->forks + i;
		philos[i].right_fork = d_ptr->forks + ((i + 1) % d_ptr->count);
		philos[i].data = d_ptr;
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
	memset(philos, 0, sizeof(t_philo) * d_ptr->count);
	d_ptr->forks = forks;
	_init_philos(d_ptr, philos);
	set_shared(&d_ptr->sim_state, SH_SET, true);
	d_ptr->start_time = date_now();
	d_ptr->philos = philos;
}
