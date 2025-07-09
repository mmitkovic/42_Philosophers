/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmitkovi <mmitkovi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 16:18:11 by mmitkovi          #+#    #+#             */
/*   Updated: 2025/07/09 15:43:37 by mmitkovi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_gets_forks(t_philo *philo)
{
	if (philo->philo_id % 2 == 0)
	{
		printf("%d has taken a fork\n", philo->philo_id);
		fflush(stdout);
		pthread_mutex_lock(philo->rightFork);
		pthread_mutex_lock(philo->leftFork);
	}
	else
	{
		printf("%d has taken a fork\n", philo->philo_id);
		fflush(stdout);
		pthread_mutex_lock(philo->leftFork);
		pthread_mutex_lock(philo->rightFork);
	}
	return (1);
}
void	philo_eats(t_philo *philo)
{
	philo->meals_eaten++;
	printf("%d is eating\n", philo->philo_id);
	fflush(stdout); // Force the buffer to write to the file NOW!
	//printf("usleep has val: %lu\n", philo->table->time_to_eat);
	usleep(philo->table->time_to_eat * 1000);
	if (philo->meals_eaten == philo->table->num_must_eat)
	{
		// We should break simulation
	}
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
	while (!philo->table->simulation_should_end)
    {
		if (philo_gets_forks(philo))
		{
			/* --- THINK --- */
			// 1. EAT: Try to pick up forks and eat
        	// 2. SLEEP: Release the forks and sleep
        	// 3. THINK: Announce that it's thinking
			philo_eats(philo);
			philo_release_forks(philo);
			usleep(200);
		}
    }
    return (NULL);
}

int	main(int ac, char **av)
{
	if (ac != 5 && ac != 6)
	{
		printf("Usage: [num_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep]\n");
		return (1);
	}
	t_philo	*philo;
	t_table	*table;
	int	i;
	/* --- INIT TABLE --- */
	table = init_table(table, av);
	mutex_init(table);
	/* --- INIT PHILOS --- */
	philo = init_and_start_threads(table, philo, av);
	i = 0;
	while (i < table->num_of_philo)
	{
		pthread_join(philo[i].thread_handle, NULL);
		i++;
	}
	return (0);
}
