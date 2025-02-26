/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:08:26 by sliziard          #+#    #+#             */
/*   Updated: 2025/02/26 16:44:33 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
#define PHILO_H

# include <pthread.h>
# include <stdbool.h>

# define ERROR_ARG_MSG "Error: Wrong number of arguments. Usage: ./philo \
<number_of_philosophers <time_to_die> <time_to_eat> <time_to_sleep> \
[number_of_times_each_philosopher_must_eat]"

typedef enum e_paction
{
	ACT_FORK,
	ACT_EAT,
	ACT_SLEEP,
	ACT_THINK,
	ACT_DIE,
	ACT_SIZE
}	t_paction;

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

typedef struct s_data
{
	int 	count;
	int			 	time_to_die;
	int			 	time_to_eat;
	int			 	time_to_sleep;
	int			 	must_eat_count;
	long			start_time;
	bool		 	sim_end;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	*forks;
	t_philo			*philos;
}	t_data;

//* Parse
int	ft_satoi(char const *nptr, int *error);

//* Init
int	init_data(t_data *d_ptr, int ac, char const *av[]);

//* Utils
long	date_now();
void	philog(t_paction act, int id);

#endif
