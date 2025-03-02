/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:08:26 by sliziard          #+#    #+#             */
/*   Updated: 2025/03/02 18:58:27 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
#define PHILO_H

# include <pthread.h>
# include <stdbool.h>

# define ERR_ARG_NB "Error: Wrong number of arguments. Usage: ./philo \
<number_of_philosophers <time_to_die> <time_to_eat> <time_to_sleep> \
[number_of_times_each_philosopher_must_eat]"

# define ERR_INVALID_ARG "Error: Wrong arguments, each one should fit in an \
unsigned int (except the last one [optional one] that need to fit in a int)\n"

# define MONITOR_DELAY 10000 //? 10 milliseconds

typedef enum e_paction
{
	ACT_FORK,
	ACT_EAT,
	ACT_SLEEP,
	ACT_THINK,
	ACT_DIE,
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
	unsigned int 	count;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	int			 	must_eat_count;
	long			start_time;
	bool		 	sim_end;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	*forks;
	t_philo			*philos;
}	t_data;

// init
int		init_data(t_data *d_ptr, int ac, char const *av[]);

// routine
void	*philo_life(void *param);

// utils
long	date_now();
void	philog(t_philo phi, t_paction state);

#endif
