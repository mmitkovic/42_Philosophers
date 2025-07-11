/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmitkovi <mmitkovi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 16:18:16 by mmitkovi          #+#    #+#             */
/*   Updated: 2025/07/11 15:15:34 by mmitkovi         ###   ########.fr       */
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
	unsigned long	time_to_die;
	unsigned long	time_to_eat;
	unsigned long	time_to_sleep;
	int				num_must_eat;
	/* --- Simulation State --- */
	unsigned long long	start_time;
	int				simulation_should_end; // A flag to stop the simulation
	/* --- Resources --- */
	pthread_mutex_t	table_lock;
	pthread_mutex_t	*forks; // An array of mutexes, one for each fork
	struct s_philo	*philos;
}	t_table;

typedef struct 	s_philo
{
	/* --- Personal Info --- */
	int					philo_id;
	int					meals_eaten;
	pthread_mutex_t		meals_lock;
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
int	check_valid_args(char **av);
int	input_check(int ac, char **av);
long long ft_time_in_ms(void);

/* --- INIT --- */
t_table	*init_table(t_table *table, char **av);
t_philo	*init_and_start_threads(t_table *table, t_philo *philo, char **av);
void	mutex_init(t_table *table);

/* --- ROUTINE --- */
void *philosopher_routine(void *arg);
void	*supervisor_routine(void *arg);

# endif