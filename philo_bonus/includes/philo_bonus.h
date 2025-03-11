/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:08:26 by sliziard          #+#    #+#             */
/*   Updated: 2025/03/11 17:20:05 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <semaphore.h>
# include <unistd.h>
# include <stdbool.h>

# define USLEEP_INTERVAL 500
# define MONITOR_DELAY 50 
# define ODD_DELAY 200
# define THINK_DELAY 50

# define ERR_ARG_NB "Error: Wrong number of arguments. Usage: ./philo \
<number_of_philosophers <time_to_die> <time_to_eat> <time_to_sleep> \
[number_of_times_each_philosopher_must_eat]"
# define ERR_INVALID_ARG "Error: Wrong arguments, each one should fit in an \
unsigned int (the last one need to be greater than 0)\n"
# define ERR_ALLOC "Error: Allocation failed\n"

# define SEM_PRINT	"/childs_print_sem"
# define SEM_MEALS	"/finished_meals_sem"
# define SEM_START	"/start_simulation_sem"
# define SEM_END	"/end_simulation_sem"
# define SEM_FORKS	"/forks_sem"

typedef enum e_paction
{
	ACT_FORK,
	ACT_EAT,
	ACT_SLEEP,
	ACT_THINK,
	ACT_DIE,
}	t_paction;

typedef struct s_data
{
	unsigned int	count;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	int				must_eat_count;
	long			start_time;
	sem_t			*sem_print;
	sem_t			*sem_start;
	sem_t			*sem_end;
	sem_t			*sem_meals_finished;
	sem_t			*forks;
}	t_data;

typedef struct s_philo {
	unsigned int	id;
	unsigned int	meals;
	long			lst_meal_time;
	long			next_meal_time;
	struct s_data	*__dptr;
}	t_philo;


// init
int		init_data(t_data *d_ptr, int ac, char const *av[]);
pid_t	*init_childs(t_data *data);

// childs
int		children_process(unsigned int id, t_data data);

// utils
long	date_now(void);
void	philog(t_philo owner, t_paction state);
int		ft_usleep(size_t milliseconds);
void	close_sems(t_data *d_ptr);

#endif
