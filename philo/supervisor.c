/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supervisor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmitkovi <mmitkovi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 17:58:13 by mmitkovi          #+#    #+#             */
/*   Updated: 2025/07/18 16:59:20 by mmitkovi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_starvation(t_table *table, int i)
{
	pthread_mutex_lock(&table->table_lock);
	if (ft_time_in_ms() - table->philos[i].last_meal_eaten > table->time_to_die)
	{
		pthread_mutex_unlock(&table->table_lock);
		pthread_mutex_lock(&table->table_lock);
		table->simulation_should_end = 1;
		printf("%llu %d died\n", ft_time_in_ms() - table->start_time,
			table->philos[i].philo_id + 1);
		pthread_mutex_unlock(&table->table_lock);
		return (1);
	}
	pthread_mutex_unlock(&table->table_lock);
	return (0);
}

int	philos_full(t_table *table, int all_philos_are_full, int i)
{
	pthread_mutex_lock(&table->table_lock);
	if (table->num_must_eat > 0
		&& table->philos[i].meals_eaten < table->num_must_eat)
	{
		all_philos_are_full = 0;
	}
	pthread_mutex_unlock(&table->table_lock);
	return (all_philos_are_full);
}

static int	are_philos_full(t_table *table, int all_philos_are_full)
{
	if (table->num_must_eat > 0 && all_philos_are_full)
	{
		pthread_mutex_lock(&table->table_lock);
		table->simulation_should_end = 1;
		pthread_mutex_unlock(&table->table_lock);
		return (1);
	}
	return (0);
}

void	*supervisor_routine(void *arg)
{
	t_table	*table;
	int		i;
	int		all_philos_are_full;

	table = (t_table *)arg;
	while (1)
	{
		i = -1;
		all_philos_are_full = 1;
		while (++i < table->num_of_philo)
		{
			if (check_starvation(table, i))
				return (NULL);
			all_philos_are_full = philos_full(table, all_philos_are_full, i);
		}
		if (are_philos_full(table, all_philos_are_full))
			return (NULL);
		usleep(10);
	}
	return (NULL);
}
