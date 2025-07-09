/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmitkovi <mmitkovi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 16:18:16 by mmitkovi          #+#    #+#             */
/*   Updated: 2025/07/09 15:53:10 by mmitkovi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	PHILO_H
# define PHILO_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>

typedef struct s_table
{
	/* --- Arguments from command line --- */
	int				num_of_philo;
	unsigned long	time_to_die; // av[2]
	unsigned long	time_to_eat; // av[3]
	unsigned long	time_to_sleep; // av[4]
	int				num_must_eat; // The optional argument
	/* --- Simulation State --- */
	int				simulation_should_end; // A flag to stop the simulation
	pthread_mutex_t	meal_lock; // Protects access to the 'simulation_should_end' flag
	/* --- Resources --- */
	pthread_mutex_t	*forks; // An array of mutexes, one for each fork

}	t_table;

typedef struct 	s_philo
{
	/* --- Personal Info --- */
	int					philo_id;
	int					meals_eaten;
	/* --- Starvation Tacking --- */
	unsigned long		last_meal_eaten;
	/* --- Assigned Resources --- */
	pthread_mutex_t					*leftFork;
	pthread_mutex_t					*rightFork;
	/* --- Thread Handle --- */
	pthread_t	thread_handle;
	t_table		*table;
}				t_philo;

/* --- UTILS --- */
long	ft_atol(const char *str);

/* --- INIT --- */
t_table	*init_table(t_table *table, char **av);
t_philo	*init_and_start_threads(t_table *table, t_philo *philo, char **av);
void	mutex_init(t_table *table);

/* --- ROUTINE --- */
void *philosopher_routine(void *arg);

# endif