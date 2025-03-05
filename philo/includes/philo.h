/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:08:26 by sliziard          #+#    #+#             */
/*   Updated: 2025/03/05 11:40:19 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdbool.h>

/**
 * * Whe have `data` that countains n-count of:
 * ?-	t_philo (32 | 48)
 * ?-	t_mutex (40 | 56)
 * 
 * The stack is allocated (generaly) with 8 Mo (8 388 608 bytes)
 * ? Arch 32-bytes
 * 105 + count * (32 + 40) bytes
 * * = 116510 PHILO_MAX
 * ? Arch 64-bytes
 * 153 + count * (48 + 56) bytes
 * * = 80643 PHILO_MAX
 */
# ifndef ALLOC_MODE
#  ifndef PHILO_MAX
#   define PHILO_MAX 200
#  else
#   if PHILO_MAX > 80000
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

/**
 * * Philosopher structure size
 * id:				int (2 | 4)
 * meals_eaten:		int (2 | 4)
 * last_meal_time:	long (8)
 * thread:			pthread_t (8)
 * left_fork:		ptr (4 | 8)
 * right_fork:		ptr (4 | 8)
 * data:			ptr (4 | 8)
 * * = 32 | 48 bytes
 */
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

/**
 * * Data structure size
 * ? One mutex ~= 40 | 56 bytes
 * unsigned int:	(2 | 4) * 3
 * int:				(2 | 4) * 1
 * __sim_end:		(1)
 * start_time:		(8)
 * t_mutex:			(40 | 56) * 2
 * ptr:				(4 | 8) * 2
 * * = 105 | 153 bytes 
 */
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

// init
void			init_philo_and_forks(t_data *d_ptr, t_philo *philos, \
	t_mutex *forks);
int				init_data(t_data *d_ptr, int ac, char const *av[]);

// routine
void			*philo_life(void *param);

// utils
long			date_now(void);
bool			philog(t_philo phi, t_paction state);
int				ft_usleep(size_t milliseconds);
void			set_sim_end(t_data *d_ptr, bool value);
bool			get_sim_end(t_data *d_ptr);

#endif
