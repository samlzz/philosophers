/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:08:26 by sliziard          #+#    #+#             */
/*   Updated: 2025/06/30 16:05:30 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <semaphore.h>
# include <unistd.h>
# include <stdbool.h>
# include <stdint.h>

# define USLEEP_INTERVAL 500

# define ERR_ARG_NB "Error: Wrong number of arguments. Usage: ./philo \
<number_of_philosophers <time_to_die> <time_to_eat> <time_to_sleep> \
[number_of_times_each_philosopher_must_eat]"
# define ERR_INVALID_ARG "Error: Wrong arguments, each one should fit in an \
unsigned int (the last one need to be greater than 0)\n"

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
	uint32_t	count;
	uint32_t	time_to_die;
	uint32_t	time_to_eat;
	uint32_t	time_to_sleep;
	int32_t		must_eat_count;
	int64_t		start_time;
	sem_t		*sem_print;
	sem_t		*sem_start;
	sem_t		*sem_end;
	sem_t		*sem_sated;
	sem_t		*forks;
}	t_data;

typedef struct s_philo
{
	uint32_t		id;
	uint32_t		meals;
	int64_t			lst_meal_time;
	int64_t			next_meal_time;
	struct s_data	*__dptr;
}	t_philo;

// init
int16_t	init_data(t_data *d_ptr, int32_t ac, char const *av[]);
pid_t	*init_childs(t_data *data);

// childs
int16_t	children_process(uint32_t id, t_data data);

// utils
int64_t	date_now(void);
void	philog(t_philo owner, t_paction state, bool ign_sem);
void	ft_usleep(size_t milliseconds);
void	close_sems(t_data *d_ptr);

#endif
