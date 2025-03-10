/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:08:26 by sliziard          #+#    #+#             */
/*   Updated: 2025/03/10 19:55:51 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdbool.h>
# include <stdint.h>

# ifndef STACK_SIZE
#  define STACK_SIZE 8388608
# endif

# ifndef ALLOC_MODE
#  ifndef PHILO_MAX
#   define PHILO_MAX 200
#  else
#   if !defined MUTEX_SIZE || !defined PHILO_SIZE || !defined DATA_SIZE
#    error "The size of mutex, philo or data are unknown, please use makefile"
#   endif
#   if PHILO_SIZE + MUTEX_SIZE == 0
#    error "Invalid value for PHILO_SIZE or MUTEX_SIZE"
#   endif
#   if PHILO_MAX > ((STACK_SIZE - DATA_SIZE - 32768) / \
(PHILO_SIZE + MUTEX_SIZE))
#    warning the PHILO_MAX provided is too large, use heap instead of stack \
(define ALLOC_MODE to delete this warning)
#    define ALLOC_MODE
#   endif
#  endif
# endif

# define USLEEP_INTERVAL 500
# define MONITOR_DELAY 10

# define ERR_ARG_NB "Error: Wrong number of arguments. Usage: ./philo \
<number_of_philosophers <time_to_die> <time_to_eat> <time_to_sleep> \
[number_of_times_each_philosopher_must_eat]"
# define ERR_INVALID_ARG "Error: Wrong arguments, each one should fit in an \
unsigned int (the last one need to be greater than 0)\n"
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
	int32_t			id;
	int32_t			meals_eaten;
	uint64_t		last_meal_time;
	uint64_t		next_meal_time;
	pthread_t		thread;
	t_mutex			*left_fork;
	t_mutex			*right_fork;
	struct s_data	*data;
}	t_philo;

typedef enum e_shared_ope{
	SH_SET,
	SH_INCREMENT,
	SH_DECREMENT
}	t_shared_ope;

typedef struct s_shared
{
	int32_t	__val;
	t_mutex	mtx;
}	t_shared;

typedef struct s_data
{
	uint32_t	count;
	uint32_t	time_to_die;
	uint32_t	time_to_eat;
	uint32_t	time_to_sleep;
	int32_t		must_eat_count;
	uint64_t	start_time;
	t_shared	sim_state;
	t_shared	sated;
	t_mutex		print_mutex;
	t_mutex		*forks;
	t_philo		*philos;
}	t_data;

// init
void		init_philo_and_forks(t_data *d_ptr, t_philo *philos, \
		t_mutex *forks);
int32_t		init_data(t_data *d_ptr, int ac, char const *av[]);

// routine
void		*philo_life(void *param);

// utils
uint64_t	date_now(void);
void		philog(t_philo phi, t_paction state);
void		ft_usleep(size_t milliseconds);
void		set_shared(t_shared	*__ptr, t_shared_ope ope, int32_t value);
int32_t		get_shared(t_shared from);

#endif
