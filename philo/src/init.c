/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 15:47:49 by sliziard          #+#    #+#             */
/*   Updated: 2025/02/25 15:51:02 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include "limits.h"

int	ft_satoi(char const *nptr, int *error)
{
	long	r;
	int		s;

	r = 0;
	s = 1;
	*error = 0;
	if (*nptr == '-' || *nptr == '+')
	{
		if (*nptr == '-')
			s = -1;
		nptr++;
	}
	while (*nptr >= '0' && *nptr <= '9' && *nptr)
	{
		r = (r * 10) + *nptr - '0';
		if ((s == 1 && r > INT_MAX) || (s == -1 && (r * -1) < INT_MIN))
		{
			*error = 1;
			return ((s == 1) * INT_MAX + (s != 1) * INT_MIN);
		}
		nptr++;
	}
	if (*nptr || *(nptr - 1) == '-' || *(nptr - 1) == '+')
		*error = 1;
	return ((int)r * s);
}