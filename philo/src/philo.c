/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:08:29 by sliziard          #+#    #+#             */
/*   Updated: 2025/02/26 15:21:13 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void	_free_for_quit(t_data *d_ptr)
{
	int	i;

	//todo manage philos threads
	free(d_ptr->philos);
	i = 0;
	while (i < d_ptr->count)
		pthread_mutex_destroy(d_ptr->forks + i++);
	free(d_ptr->forks);
	pthread_mutex_destroy(&d_ptr->print_mutex);
}

//todo: Temporaire
void print_data(t_data *data)
{
    int i;

    printf("===== Simulation Data =====\n");
    printf("Number of philosophers: %d\n", data->count);
    printf("Time to die: %d ms\n", data->time_to_die);
    printf("Time to eat: %d ms\n", data->time_to_eat);
    printf("Time to sleep: %d ms\n", data->time_to_sleep);
    printf("Meals required: %d\n", data->must_eat_count);
    printf("Start time: %ld ms\n", data->start_time);
    printf("Simulation end flag: %d\n", data->sim_end);
    printf("\n");

    printf("===== Philosophers Data =====\n");
    for (i = 0; i < data->count; i++)
    {
        printf("Philosopher %d:\n", data->philos[i].id);
        printf("  - Meals eaten: %d\n", data->philos[i].meals_eaten);
        printf("  - Last meal time: %ld ms\n", data->philos[i].last_meal_time);
        printf("  - Left fork address: %p\n", (void *)data->philos[i].left_fork);
        printf("  - Right fork address: %p\n", (void *)data->philos[i].right_fork);
        printf("\n");
    }
}

int main(int argc, char const *argv[])
{
	t_data	data;

	if (argc < 5 || argc > 6)
		return (write(2, ERROR_ARG_MSG, 160), 1);
	if (init_data(&data, argc - 1, argv + 1))
		return (1);
	print_data(&data);
	_free_for_quit(&data);
	return (0);
}
