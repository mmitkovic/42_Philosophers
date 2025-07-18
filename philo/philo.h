/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmitkovi <mmitkovi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 16:18:16 by mmitkovi          #+#    #+#             */
/*   Updated: 2025/07/18 14:27:17 by mmitkovi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_table
{
	/* --- Arguments from command line --- */
	int					num_of_philo;
	unsigned long		time_to_die;
	unsigned long		time_to_eat;
	unsigned long		time_to_sleep;
	int					num_must_eat;
	/* --- Simulation State --- */
	unsigned long long	start_time;
	int					simulation_should_end;
	/* --- Resources --- */
	pthread_mutex_t		table_lock;
	pthread_mutex_t		*forks;
	struct s_philo		*philos;
}						t_table;

typedef struct s_philo
{
	/* --- Personal Info --- */
	int					philo_id;
	int					meals_eaten;
	pthread_mutex_t		meals_lock;
	/* --- Starvation Tacking --- */
	unsigned long		last_meal_eaten;
	/* --- Assigned Resources --- */
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	/* --- Thread Handle --- */
	pthread_t			thread_handle;
	t_table				*table;
}						t_philo;

/* --- UTILS --- */
long					ft_atol(const char *str);
int						check_valid_args(char **av);
int						input_check(int ac, char **av);
long long				ft_time_in_ms(void);

/* --- INIT --- */
t_table					*init_table(char **av);
// t_philo	*init_and_start_threads(t_table *table, char **av);
void					mutex_init(t_table *table);
void					start_threads(t_philo *philo, t_table *table);
t_philo					*init_philos(t_philo *philo, t_table *table);
int						start_init(t_philo **philo, t_table **table, char **av);

/* --- ACTIONS (Mutexes) --- */
void					philo_release_forks(t_philo *philo);
void					philo_eats(t_philo *philo);

/* --- ROUTINE --- */
void					*philosopher_routine(void *arg);
void					*supervisor_routine(void *arg);
int						philo_gets_forks(t_philo *philo);

/* --- HELPERS --- */
void					print_status(t_philo *philo, char *status);
void					philo_delay(t_table *table,
							unsigned long delay_duration_ms);
int						is_simulation_over(t_philo *philo);
int						one_philo(char **av);

/* --- SUPERVISOR --- */
int						check_starvation(t_table *table, int i);
int						philos_full(t_table *table, int all_philos_are_full,
							int i);

#endif