/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 12:46:40 by sliziard          #+#    #+#             */
/*   Updated: 2025/03/06 17:00:59 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>

void	destroy_data(void *ptr)
{
	if (ptr)
		free(ptr);
	sem_unlink(SEM_END);
	sem_unlink(SEM_PRINT);
	sem_unlink(SEM_FORKS);
}

void	wait_childrens(pid_t *childs, size_t count)
{
	size_t	i;

	i = 0;
	while (i < count)
		kill(childs[i++], SIGTERM);
	i = 0;
	while (i < count)
		waitpid(childs[i++], NULL, 0);
}

int	main(int argc, char const *argv[])
{
	t_data	data;
	sem_t	*end;
	pid_t	*childs;

	if (argc < 5 || argc > 6)
		return (write(2, ERR_ARG_NB, 160), 1);
	if (init_data(&data, argc - 1, argv + 1))
	{
		write(2, ERR_INVALID_ARG, 126);
		return (1);
	}
	end = sem_open(SEM_END, O_CREAT, 0644, 0);
	childs = init_childs(data);
	if (!childs)
		return (1);
	sem_wait(end);
	wait_childrens(childs, data.count);
	sem_close(end);
	destroy_data(childs);
	return (0);	
}
