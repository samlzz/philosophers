/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:08:29 by sliziard          #+#    #+#             */
/*   Updated: 2025/02/25 20:23:26 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>
#include <stdio.h>

void	_free_for_quit(t_data *d_ptr)
{
	int	i;

	//todo free ou libérer philos data
	free(d_ptr->philos);
	i = 0;
	while (i < d_ptr->nb_philos)
		pthread_mutex_destroy(d_ptr->forks + i++);
	free(d_ptr->forks);
	pthread_mutex_destroy(d_ptr->print_mutex);
}

int main(int argc, char const *argv[])
{
	t_data	data;

	if (argc < 5 || argc > 6)
		return (write(2, ERROR_ARG_MSG, 160), 1);
	init_data(&data, argc - 1, argv + 1);
	return 0;
}
