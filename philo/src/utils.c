/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:41:35 by sliziard          #+#    #+#             */
/*   Updated: 2025/02/26 16:44:11 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <sys/time.h>

long	date_now()
{
	struct timeval tv;
	
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000);
}

void	philog(t_paction act, int id)
{
	char	*strs[ACT_SIZE];

	strs[ACT_FORK] = "has taken a fork";
	strs[ACT_EAT] = "is eating";
	strs[ACT_SLEEP] = "is sleeping";
	strs[ACT_THINK] = "is thinking";
	strs[ACT_DIE] = "is died";
	printf("%ld %d %s\n", date_now(), id, strs[act]);
}
