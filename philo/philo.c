/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmitkovi <mmitkovi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 16:18:11 by mmitkovi          #+#    #+#             */
/*   Updated: 2025/07/16 14:26:51 by mmitkovi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>
/*--- LAST PHILO PICK UP THE LEFT FORK ---*/
// int	philo_gets_forks(t_philo *philo)
// {
// 	//philo->table->num_of_philo
// 	//philo->philo_id
// 	if (philo->philo_id == philo->table->num_of_philo - 1)
// 	{
// 		pthread_mutex_lock(philo->leftFork);
// 		print_status(philo, "has taken a fork");
// 		printf("Philo %d Waiting: %llu\n",philo->philo_id + 1, ft_time_in_ms() - philo->table->start_time);
// 		pthread_mutex_lock(philo->rightFork);
// 		print_status(philo, "has taken a fork");
// 	}
// 	else
// 	{
// 		/**/
// 		pthread_mutex_lock(philo->rightFork);
// 		print_status(philo, "has taken a fork");
// 		printf("Philo %d Waiting: %llu\n",philo->philo_id + 1, ft_time_in_ms() - philo->table->start_time);
// 		pthread_mutex_lock(philo->leftFork);
// 		print_status(philo, "has taken a fork");
// 	}
// 	return (1);
// }

int	philo_gets_forks(t_philo *philo)
{
	if (philo->philo_id % 2 == 0)
	{
		pthread_mutex_lock(philo->rightFork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->leftFork);
		print_status(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->leftFork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->rightFork);
		print_status(philo, "has taken a fork");
	}
	return (1);
}
void	philo_eats(t_philo *philo)
{
	pthread_mutex_lock(&philo->meals_lock);
	philo->last_meal_eaten = (unsigned long)ft_time_in_ms();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meals_lock);
	philo_delay(philo, philo->table->time_to_eat);
	//usleep(philo->table->time_to_eat * 1000);
}
void	philo_release_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->rightFork);
	pthread_mutex_unlock(philo->leftFork);
}
void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (!philo->table->simulation_should_end)
	{
		print_status(philo, "is thinking");
		philo_gets_forks(philo);
		// pthread_mutex_lock(&philo->table->table_lock);
		if (philo->table->simulation_should_end)
		{
			philo_release_forks(philo);
			//pthread_mutex_unlock(&philo->table->table_lock);
			break ;
		}
		// pthread_mutex_unlock(&philo->table->table_lock);
		print_status(philo, "is eating");
		philo_eats(philo);
		philo_delay(philo, philo->table->time_to_eat);
		philo_release_forks(philo);
		print_status(philo, "is sleeping");
		usleep(philo->table->time_to_sleep * 1000);
	}
	return (NULL);
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
			pthread_mutex_lock(&table->philos[i].meals_lock);
			// Check if philo has starved
			if (ft_time_in_ms()
				- table->philos[i].last_meal_eaten > table->time_to_die)
			{
				pthread_mutex_lock(&table->table_lock);
				table->simulation_should_end = 1;
				printf("%llu %d died\n", ft_time_in_ms()
					- table->start_time,
					table->philos[i].philo_id + 1);
				pthread_mutex_unlock(&table->table_lock);
				pthread_mutex_unlock(&table->philos[i].meals_lock);
				return (NULL);
			}
			// check if philo still needs to eat
			if (table->num_must_eat > 0
				&& table->philos[i].meals_eaten < table->num_must_eat)
				all_philos_are_full = 0;
			pthread_mutex_unlock(&table->philos[i].meals_lock);
		}
		// Check if the "all philosophers have eaten enough" condition is met.
		if (table->num_must_eat > 0
			&& all_philos_are_full)
		{
			pthread_mutex_lock(&table->table_lock);
			table->simulation_should_end = 1;
			pthread_mutex_unlock(&table->table_lock);
			return (NULL); // Exit supervisor, simulation is over.
		}
		usleep(10); // Wait a moment before checking again to prevent traffic rush
	}
	return (NULL);
}

int	main(int ac, char **av)
{
	t_philo				*philo;
	t_table				*table;
	pthread_t			supervisor;
	unsigned long long	start_time;
	int					i;

	if (input_check(ac, av))
		return (1);
	if (start_init(&philo, &table, av) == 1)
		return (1);
	mutex_init(table);
	init_philos(philo, table);
	start_threads(philo, table);
	pthread_create(&supervisor, NULL, supervisor_routine, table);
	int j = 0;
	while (j < table->num_of_philo)
		pthread_join(philo[j++].thread_handle, NULL);
	pthread_join(supervisor, NULL);
	free(table->forks);
	free(philo);
	free(table);
	return (0);
}
