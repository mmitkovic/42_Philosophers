/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmitkovi <mmitkovi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 16:29:18 by mmitkovi          #+#    #+#             */
/*   Updated: 2025/07/16 14:13:27 by mmitkovi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_status(t_philo *philo, char *status)
{
	pthread_mutex_lock(&philo->table->table_lock);
	if (!philo->table->simulation_should_end)
		printf("%llu %d %s\n", ft_time_in_ms() - philo->table->start_time,
			philo->philo_id + 1, status);
	pthread_mutex_unlock(&philo->table->table_lock);
}

static long int	get_time(long int start_time)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (((time.tv_sec - start_time) * 1000) + (time.tv_usec / 1000));
}

void	philo_delay(t_philo *philo, long int delay)
{
	long int	start;

	start = get_time(philo->table->start_time);
	while (get_time(philo->table->start_time) - start < delay)
		usleep(20);
}
