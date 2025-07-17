#include "philo.h"

void	mutex_init(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->num_of_philo)
	{
		pthread_mutex_init(&table->forks[i], NULL);
		i++;
	}
}

t_table	*init_table(char **av)
{
	t_table	*table;

	table = (t_table *)malloc(1 * sizeof(t_table));
	if (!table)
		return (NULL);
	table->num_of_philo = ft_atol(av[1]);
	table->time_to_die = ft_atol(av[2]);
	table->time_to_eat = ft_atol(av[3]);
	table->time_to_sleep = ft_atol(av[4]);
	if (av[5] != NULL)
		table->num_must_eat = ft_atol(av[5]);
	table->start_time = ft_time_in_ms();
	table->simulation_should_end = 0;
	pthread_mutex_init(&table->table_lock, NULL);
	table->forks = malloc(table->num_of_philo * sizeof(pthread_mutex_t));
	if (!table->forks)
		return (NULL);
	return (table);
}
t_philo	*init_philos(t_philo *philo, t_table *table)
{
	int	i;

	i = 0;
	while (i < table->num_of_philo)
	{
		philo[i].philo_id = i;
		philo[i].meals_eaten = 0;
		philo[i].last_meal_eaten = table->start_time;
		philo[i].leftFork = &table->forks[i];
		if (i == 0)
			philo[i].rightFork = &table->forks[table->num_of_philo - 1];
		else
			philo[i].rightFork = &table->forks[i - 1];
		philo[i].table = table;
		pthread_mutex_init(&philo[i].meals_lock, NULL);
		i++;
	}
	return (philo);
}
void	start_threads(t_philo *philo, t_table *table)
{
	int	i;

	i = 0;
	while (i < table->num_of_philo)
	{
		pthread_create(&philo[i].thread_handle, NULL, philosopher_routine, &philo[i]);
		printf("Waiting...\n");
		usleep(philo[i].philo_id * 10);
		i++;
	}
}

int	start_init(t_philo **philo, t_table **table, char **av)
{
	*philo = NULL;
	*table = NULL;
	*table = init_table(av);
	*philo = malloc((*table)->num_of_philo * sizeof(t_philo));
	if (!*philo)
		return (1);
	(*table)->philos = *philo;
	return (0);
}
