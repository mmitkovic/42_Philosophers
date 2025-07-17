/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmitkovi <mmitkovi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 16:29:18 by mmitkovi          #+#    #+#             */
/*   Updated: 2025/07/17 19:21:09 by mmitkovi         ###   ########.fr       */
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

void	philo_delay(t_table *table, unsigned long delay_duration_ms)
{
	long long	start_of_delay;

	start_of_delay = ft_time_in_ms();
	// Loop while the elapsed time is less than the desired delay
	while ((ft_time_in_ms() - start_of_delay) < (long long)delay_duration_ms)
	{
		// Check if the simulation ended during the delay.
		// This makes philosophers stop their actions more quickly when one dies.
		pthread_mutex_lock(&table->table_lock);
		if (table->simulation_should_end)
		{
			pthread_mutex_unlock(&table->table_lock);
			break ;
		}
		pthread_mutex_unlock(&table->table_lock);
		// Sleep for a short period to prevent the CPU from running at 100%
		usleep(100);
	}
}
int	is_simulation_over(t_philo *philo)
{
	int result;

	pthread_mutex_lock(&philo->table->table_lock);
	result = philo->table->simulation_should_end;
	pthread_mutex_unlock(&philo->table->table_lock);
	return (result);
}