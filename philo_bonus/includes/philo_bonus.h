/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:08:26 by sliziard          #+#    #+#             */
/*   Updated: 2025/03/05 14:29:21 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <pthread.h>
# include <semaphore.h>
# include <stdbool.h>

# define USLEEP_INTERVAL 500
# define MONITOR_DELAY 10

# define ERR_ARG_NB "Error: Wrong number of arguments. Usage: ./philo \
<number_of_philosophers <time_to_die> <time_to_eat> <time_to_sleep> \
[number_of_times_each_philosopher_must_eat]"
# define ERR_INVALID_ARG "Error: Wrong arguments, each one should fit in an \
unsigned int (except the last one [optional one] that need to fit in a int)\n"
# define ERR_ALLOC "Error: Allocation failed\n"

typedef enum e_paction
{
	ACT_FORK,
	ACT_EAT,
	ACT_SLEEP,
	ACT_THINK,
	ACT_DIE,
}	t_paction;

typedef pthread_mutex_t	t_mutex;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long			last_meal_time;
	t_mutex			meal_mutex;
	pthread_t		thread;
	t_mutex			*left_fork;
	t_mutex			*right_fork;
	struct s_data	*data;
}	t_philo;

typedef struct s_data
{
	unsigned int	count;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	int				must_eat_count;
	long			start_time;
	int				__sim_end : 1;
	t_mutex			end_mutex;
	t_mutex			print_mutex;
	t_mutex			*forks;
	t_philo			*philos;
}	t_data;

// utils
long			date_now(void);
bool			philog(t_philo phi, t_paction state);
int				ft_usleep(size_t milliseconds);
void			set_sim_end(t_data *d_ptr, bool value);
bool			get_sim_end(t_data *d_ptr);

#endif
