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
	usleep(philo->table->time_to_sleep * 1000);
}
void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (!philo->table->simulation_should_end)
	{
		if (philo_gets_forks(philo))
		{
			// 1. EAT: Try to pick up forks and eat
			// 2. SLEEP: Release the forks and sleep
			// 3. THINK: Announce that it's thinking
			philo_eats(philo);
			philo_release_forks(philo);
			usleep(100);
			// TODO Thinking
		}
	}
	return (NULL);
}

void	*supervisor_routine(void *arg)
{
	t_table	*table;
	int		i;
	int		full_philos = 0;

	table = (t_table *)arg;
	while (!table->simulation_should_end)
	{
		i = 0;
		while (i < table->num_of_philo)
		{
			pthread_mutex_lock(&table->philos[i].meals_lock);
			// check for the death and meals eaten ...
			if (table->philos[i].meals_eaten == table->num_must_eat)
			{
				table->simulation_should_end = 1;
				printf("Meals eaten == Num Must Eat\n");
			}
			// if (ft_time_in_ms() - table->philos[i].last_meal_eaten >= table->time_to_die)
			// {
			// 	//table->simulation_should_end = 1;
			// 	printf("Philo: %d\n", table->philos[i].philo_id);
			// 	printf("Philo DIED %llu\n", ft_time_in_ms() - table->philos[i].last_meal_eaten);
			// }
			pthread_mutex_unlock(&table->philos[i].meals_lock);
			full_philos++;
			i++;
		}
		usleep(100);
		// check if all are full and sleep...
		if (full_philos == table->num_must_eat)
		{
			table->simulation_should_end = 1;
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
