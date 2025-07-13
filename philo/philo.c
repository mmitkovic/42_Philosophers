/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmitkovi <mmitkovi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 16:18:11 by mmitkovi          #+#    #+#             */
/*   Updated: 2025/07/11 15:26:25 by mmitkovi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>

int	philo_gets_forks(t_philo *philo)
{
	if (philo->philo_id % 2 == 0)
	{
		pthread_mutex_lock(philo->rightFork);
		pthread_mutex_lock(philo->leftFork);
		printf("%llu %d has taken a fork\n", ft_time_in_ms() - philo->table->start_time, philo->philo_id);
		fflush(stdout);
	}
	else
	{
		pthread_mutex_lock(philo->leftFork);
		pthread_mutex_lock(philo->rightFork);
		printf("%llu %d has taken a fork\n", ft_time_in_ms() - philo->table->start_time, philo->philo_id);
		fflush(stdout);
	}
	return (1);
}
void	philo_eats(t_philo *philo)
{
	pthread_mutex_lock(&philo->meals_lock);
	philo->last_meal_eaten = (unsigned long)ft_time_in_ms();
	printf("Current time: %llu\n", philo->last_meal_eaten - philo->table->start_time);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meals_lock);
	printf("%d is eating\n", philo->philo_id);
	fflush(stdout);
	usleep(philo->table->time_to_eat * 1000);
}
void	philo_release_forks(t_philo *philo)
{
	printf("Philo %d releases fork\n", philo->philo_id);
	fflush(stdout);
	pthread_mutex_unlock(philo->rightFork);
	pthread_mutex_unlock(philo->leftFork);
}
void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (!philo->table->simulation_should_end)
	{

		pthread_mutex_lock(&philo->table->table_lock);
		if (philo->table->simulation_should_end)
		{
			pthread_mutex_unlock(&philo->table->table_lock);
			break ;
		}
		pthread_mutex_unlock(&philo->table->table_lock);
		printf("%llu %d is thinking\n", ft_time_in_ms() - philo->table->start_time, philo->philo_id);
		if (philo_gets_forks(philo))
		{
			philo_eats(philo);
			philo_release_forks(philo);
			usleep(philo->table->time_to_sleep * 1000);
			usleep(100);
		}
	}
	return (NULL);
}

void	*supervisor_routine(void *arg)
{
	t_table	*table;
	int		i;
	int		num_philos_full;

	table = (t_table *)arg;
	while (1)
	{
		i = 0;
		num_philos_full = 0;
		while (i < table->num_of_philo)
		{
			pthread_mutex_lock(&table->philos[i].meals_lock);
			if (ft_time_in_ms() - table->philos[i].last_meal_eaten >= table->time_to_die)
			{
				pthread_mutex_lock(&table->table_lock);
				table->simulation_should_end = 1;
				pthread_mutex_unlock(&table->table_lock);
				printf("Philo DIED %llu\n", ft_time_in_ms() - table->philos[i].last_meal_eaten);
			}
			if (table->num_must_eat && table->philos[i].meals_eaten >= table->num_must_eat)
				num_philos_full++;
			pthread_mutex_unlock(&table->philos[i].meals_lock);
			i++;
		}
		usleep(100);
		// check if all are full and sleep...
		if (table->num_must_eat > 0 && num_philos_full == table->num_of_philo)
		{
			pthread_mutex_lock(&table->table_lock);
			table->simulation_should_end = 1;
			pthread_mutex_unlock(&table->table_lock);
		}
	}
	return (NULL);
}

int	main(int ac, char **av)
{
	t_philo	*philo;
	t_table	*table;
	pthread_t	supervisor;
	unsigned long long start_time;
	int		i;

	if (input_check(ac, av))
		return (1);
	table = init_table(table, av);
	mutex_init(table);
	philo = init_and_start_threads(table, philo, av);
	table->philos = philo;
	// TODO Free everything
	pthread_create(&supervisor, NULL, supervisor_routine, table);
	i = 0;
	while (i < table->num_of_philo)
	{
		pthread_join(philo[i].thread_handle, NULL);
		i++;
	}
	pthread_join(supervisor, NULL);
	return (0);
}
