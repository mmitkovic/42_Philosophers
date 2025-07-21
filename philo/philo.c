/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmitkovi <mmitkovi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 16:18:11 by mmitkovi          #+#    #+#             */
/*   Updated: 2025/07/18 16:54:58 by mmitkovi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_gets_forks(t_philo *philo)
{
	if (philo->philo_id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
	}
	return (1);
}

void	philo_eats(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->table_lock);
	philo->last_meal_eaten = (unsigned long)ft_time_in_ms();
	pthread_mutex_unlock(&philo->table->table_lock);
	pthread_mutex_lock(&philo->table->table_lock);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->table->table_lock);
	philo_delay(philo->table, philo->table->time_to_eat);
}

void	philo_release_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		print_status(philo, "is thinking");
		philo_gets_forks(philo);
		if (is_simulation_over(philo))
		{
			philo_release_forks(philo);
			break ;
		}
		print_status(philo, "is eating");
		philo_eats(philo);
		philo_release_forks(philo);
		print_status(philo, "is sleeping");
		usleep(philo->table->time_to_sleep * 1000);
	}
	return (NULL);
}

int	main(int ac, char **av)
{
	t_philo		*philo;
	t_table		*table;
	pthread_t	supervisor;
	int			j;

	if (input_check(ac, av))
		return (1);
	if (one_philo(av))
		return (0);
	if (start_init(&philo, &table, av) == 1)
		return (1);
	mutex_init(table);
	init_philos(philo, table);
	start_threads(philo, table);
	pthread_create(&supervisor, NULL, supervisor_routine, table);
	j = 0;
	while (j < table->num_of_philo)
		pthread_join(philo[j++].thread_handle, NULL);
	pthread_join(supervisor, NULL);
	free(table->forks);
	free(philo);
	free(table);
	return (0);
}
