/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmitkovi <mmitkovi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 16:30:39 by mmitkovi          #+#    #+#             */
/*   Updated: 2025/07/18 14:38:00 by mmitkovi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	ft_atol(const char *str)
{
	long	res;
	int		sign;
	int		i;

	res = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\r'
		|| str[i] == '\v' || str[i] == '\f')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + str[i] - '0';
		i++;
	}
	return ((long)(res * sign));
}

static int	ft_isnum(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] >= '0' && s[i] <= '9')
			i++;
		else
			return (0);
	}
	return (1);
}

int	check_valid_args(char **av)
{
	char	**args;
	int		i;

	args = av;
	i = 1;
	while (args[i])
	{
		if (!ft_isnum(args[i]))
			return (0);
		i++;
	}
	return (1);
}

int	input_check(int ac, char **av)
{
	if (ac != 5 && ac != 6)
	{
		printf("Usage: [num_of_philosophers] [time_to_die]"
			"[time_to_eat] [time_to_sleep]"
			"[number_of_times_each_philosopher_must_eat]\n");
		return (1);
	}
	if (!check_valid_args(av))
	{
		printf("Arguments should be digits!\n");
		return (1);
	}
	if (ac == 6 && ft_atol(av[5]) < 1)
	{
		printf("Last argument [number_of_times_each_philosopher_must_eat]"
			" has to be greater then 0\n");
		return (1);
	}
	return (0);
}

long long	ft_time_in_ms(void)
{
	struct timeval	t;
	long long		milliseconds;

	gettimeofday(&t, NULL);
	milliseconds = t.tv_sec * 1000LL + t.tv_usec / 1000;
	return (milliseconds);
}
