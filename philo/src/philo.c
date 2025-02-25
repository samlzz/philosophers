/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:08:29 by sliziard          #+#    #+#             */
/*   Updated: 2025/02/25 16:29:08 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include "unistd.h"

static bool	_set_nb_field(int *field, char const *nb)
{
	bool	has_err;

	has_err = false;
	*field = ft_satoi(nb, &has_err);
	return (has_err);
}
bool	init_data(t_data *d_ptr, int ac, char const *av)
{

	d_ptr->start_time = date_now();
	if (set_nb_field(&d_ptr->nb_philos, av[0]))
		return (false);
	if (set_nb_field(&d_ptr->time_to_die, av[1]))
		return (false);
	if (set_nb_field(&d_ptr->time_to_eat, av[2]))
		return (false);
	if (set_nb_field(&d_ptr->time_to_sleep, av[3]))
		return (false);
	if (ac == 5)
		if (set_nb_field(&d_ptr->must_eat_count, av[4]))
			return (false);
	else
		d_ptr->must_eat_count = -1;
	d_ptr->sim_end = false;
	//todo: Init
	d_ptr->forks;
	d_ptr->philos;
	d_ptr->print_mutex;
	return (true);
}

int main(int argc, char const *argv[])
{
	t_data	data;

	if (argc < 5 || argc > 6)
		return (write(2, ERROR_ARG_MSG, 160), 1);
	init_data(&data, argc - 1, argv + 1);
	return 0;
}
