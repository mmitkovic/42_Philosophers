#include "philo_bonus.h"

sem_t		sem; // semaphore object
pthread_t 	threads[5];

#define PERMIT_COUNT 2

void	*thread_fn_callback(void *arg)
{
	int i;
	char	*thread_name = (char *)arg;

	printf("%s endetring C.S.\n", thread_name);
	/* --- CS begins here --- */
	/*Depending on sem_wait counter value, the thread will be blocked or pass.*/
	sem_wait(&sem); // as soon pthread_fn executes sem_wait is decremented by 1

	i = 0;
	while (i < 5)
	{
		printf("%s executing in C.S.\n", thread_name);
		sleep(2);
		i++;
	}

	printf("%s exiting C.S.\n", thread_name);
	/*Before exiting the CS our thread increments the valuye of the semaphore counter*/
	sem_post(&sem);
	/* CS ends here*/
	printf("%s exit C.S. successfully\n", thread_name);
	return (NULL);
}

void	thread_create(pthread_t *thread_handle, void *arg)
{
	int rc = pthread_create(thread_handle, NULL, thread_fn_callback, arg);
	if (rc != 0)
	{
		printf("Error occurred, errno = %d\n", rc);
		exit(0);
	}
}

int	main(int ac, char **av)
{
	sem_init(&sem, 0, PERMIT_COUNT);
	thread_create(&threads[0], "thread0");
	//thread_create(&threads[1], "thread1");
	//thread_create(&threads[2], "thread2");
	//thread_create(&threads[3], "thread3");
	//thread_create(&threads[4], "thread4");
	sem_destroy(&sem);
	int	i = 0;
	while (i < 5)
	{
		pthread_join(threads[i], NULL);
	}
	//sem_destroy(&sem);
	return (0);
}

