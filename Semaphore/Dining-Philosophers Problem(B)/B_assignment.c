#include<pthread.h>
#include<semaphore.h>
#include<stdio.h>
#include<unistd.h>

#define N 6


int phil[N] = {0,1,2,3,4,5};

sem_t S[N];

void philosopher(int num)
{

    int philosopher_number = num;
	int right = (philosopher_number +1)%N;
	//if the philosopher is even, he first picks up the right chopstick
	if(philosopher_number %2 ==0)
	{
   	    sem_wait(&S[right]);
		printf("philosopher %d pick up %d chopstick\n", philosopher_number, right);
		//pick up left chopstick
    	sem_wait(&S[philosopher_number]);
		printf("philosopher %d pick up %d chopstick\n", philosopher_number, philosopher_number);
		//A philosopher has to take both chopsticks to eat
    	printf("\tphilosopher(%d) starting eating. \n", philosopher_number);
		sleep(2);
    	printf("\tphilosopher(%d) finishes eating. \n", philosopher_number);
    	//putdown left chopstick
    	sem_post(&S[philosopher_number]);
    	//put down right chopstick
    	sem_post(&S[right]);

	}
	else
	{
		//if the philosopher is odd, he first pick up the left chopstick
		sem_wait(&S[philosopher_number]);
        printf("philosopher %d pick up %d chopstick\n", philosopher_number, philosopher_number);
		//pick up right chopstick
		sem_wait(&S[right]);
        printf("philosopher %d pick up %d chopstick\n", philosopher_number, right);
		
		printf("\tphilosopher(%d) starting eating. \n", philosopher_number);
		sleep(2);
    	printf("\tphilosopher(%d) finishes eating. \n", philosopher_number);
		//putdown right chopstick
		sem_post(&S[right]);
		//put down left chopstick
		sem_post(&S[philosopher_number]);
	}

}

// A function that determines the number of times a philosopher function is executed on each thread
int func(void *arg)
{
	int* num = arg;
	for(int i=0;i<100;i++)
	{
		philosopher(*num);
	}

}

int main()
{
    pthread_t thread_id[N];
//semaphore to protect each chopstick
    for(int i=0;i<N;i++)
    {
        sem_init(&S[i],0,1);
    }
//create thread, run the func function and phill is the parameter of that function
    for(int i=0; i<N; i++)
    {
        pthread_create(&thread_id[i], NULL,(void*)func, &phil[i]);
    }
//wait for the thread to end
    for(int i = 0; i<N; i++)
    {
        pthread_join(thread_id[i], NULL);
    }
//destory the semaphore
    for(int i=0;i<N;i++)
    {
        sem_destroy(&S[N]);
    }

}

