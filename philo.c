/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmitkovi <mmitkovi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 16:18:11 by mmitkovi          #+#    #+#             */
/*   Updated: 2025/07/08 17:13:35 by mmitkovi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_gets_forks(t_philo *philo)
{
	if (philo->philo_id % 2 == 0)
	{
		printf("EVEN: Philo %d has fork.\n", philo->philo_id);
		fflush(stdout);
		pthread_mutex_lock(philo->rightFork);
		pthread_mutex_lock(philo->leftFork);
	}
	else
	{
		printf("ODD: Philo %d has fork.\n", philo->philo_id);
		fflush(stdout);
		pthread_mutex_lock(philo->leftFork);
		pthread_mutex_lock(philo->rightFork);
	}
	return (1);
}
void	philo_eats(t_philo *philo)
{
	philo->meals_eaten++;
	printf("Philo %d eats %d time[s]\n", philo->philo_id, philo->meals_eaten);
	fflush(stdout); // Force the buffer to write to the file NOW!
	sleep(1);
}
void	philo_release_forks(t_philo *philo)
{
	printf("Philo %d releases fork\n", philo->philo_id);
	fflush(stdout);
	pthread_mutex_unlock(philo->rightFork);
	pthread_mutex_unlock(philo->leftFork);
}
void *philosopher_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    while (1)
    {
		if (philo_gets_forks(philo))
		{
			/* --- THINK --- */
			philo_eats(philo);
			philo_release_forks(philo);
			sleep(2); // not allo
		}
    }
    return (NULL);
}

int	main(int ac, char **av)
{
	if (ac < 5)
	{
		printf("Usage: [num_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep]\n");
		return (1);
	}
	t_philo	*philo;
	t_table	*table;
	int	s, num_philos, i;

	num_philos = atoi(av[1]);
	table = (t_table *)malloc(1 * sizeof(t_table));
	if (!table)
		return (1);
	/* --- Init Table --- */
	table->num_of_philo = num_philos;
	table->time_to_die = 0;
	table->time_to_eat = 0;
	table->time_to_sleep = 0;
	table->simulation_should_end = 0;
	table->forks = malloc(num_philos * sizeof(pthread_mutex_t));
	if (!table)
		return (1);
	//pthread_mutex_init(&table->table_lock, NULL);
	i = 0;
	while (i < num_philos)
	{
		pthread_mutex_init(&table->forks[i], NULL);
		i++;
	}
	philo = malloc(num_philos * sizeof(t_philo));
		if (!philo)
			return (1);
	i = 0;
	/* Create Philo Thread */
	while (i < num_philos)
	{
		philo[i].philo_id = i;
		philo[i].meals_eaten = 0;
		philo[i].leftFork = &table->forks[i];
		if (i == 0)
			philo[i].rightFork = &table->forks[num_philos- 1];
		else
			philo[i].rightFork = &table->forks[i - 1];
		pthread_create(&philo[i].thread_handle, NULL, philosopher_routine, &philo[i]);
		usleep(100); // 100ms delay to prevent philosophers from taking left fork simultaneously???
		i++;
	}
	i = 0;
	while (i < num_philos)
	{
		pthread_join(philo[i].thread_handle, NULL);
		i++;
	}
	return (0);
}
