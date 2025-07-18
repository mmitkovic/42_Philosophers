/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmitkovi <mmitkovi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 16:29:18 by mmitkovi          #+#    #+#             */
/*   Updated: 2025/07/18 14:28:49 by mmitkovi         ###   ########.fr       */
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
	while ((ft_time_in_ms() - start_of_delay) < (long long)delay_duration_ms)
	{
		pthread_mutex_lock(&table->table_lock);
		if (table->simulation_should_end)
		{
			pthread_mutex_unlock(&table->table_lock);
			break ;
		}
		pthread_mutex_unlock(&table->table_lock);
		usleep(100);
	}
}

int	is_simulation_over(t_philo *philo)
{
	int	result;

	pthread_mutex_lock(&philo->table->table_lock);
	result = philo->table->simulation_should_end;
	pthread_mutex_unlock(&philo->table->table_lock);
	return (result);
}

int	one_philo(char **av)
{
	if (ft_atol(av[1]) == 1)
	{
		printf("0 1 is thinking\n");
		usleep(100);
		printf("1 has taken a fork\n");
		usleep(ft_atol(av[2]) * 1000);
		printf("%s 1 died\n", av[2]);
		return (1);
	}
	return (0);
}
