/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:08:26 by sliziard          #+#    #+#             */
/*   Updated: 2025/02/25 15:50:36 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
#define PHILO_H

# include <sys/time.h>
# include <pthread.h>
# include <stdbool.h>

# define ERROR_ARG_MSG "Error: Wrong number of arguments. Usage: ./philo <number_of_philosophers <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]"

typedef struct s_data
{
	int			 	nb_philos;
	int			 	time_to_die;
	int			 	time_to_eat;
	int			 	time_to_sleep;
	int			 	must_eat_count;
	long			start_time;
	bool		 	sim_end;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	*forks;
	t_philo			*philos;
} t_data;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long			last_meal_time;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	struct s_data	*data;
}	t_philo;

long	date_now();

//* Parse
int	ft_satoi(char const *nptr, int *error);

#endif
