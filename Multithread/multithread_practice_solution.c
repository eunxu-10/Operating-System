#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define ARGUMENT_NUMBER 20

//array for containing computational values for each thread
long long result[ARGUMENT_NUMBER];

//put the thread number 25000000times to each thread result array
void *ThreadFunc(void *n)
{
	long long i;
	long long number = *((long long *)n);
	printf("number = %lld\n",number);
	for(i=0; i<25000000;i++)
	{
		result[number]+=number;
	}
}
int main(void)
{
	pthread_t threadID[ARGUMENT_NUMBER];

	long long argument[ARGUMENT_NUMBER];
	long long i;

	for(i=0;i<ARGUMENT_NUMBER;i++)
	{
		argument[i] = i;
	}

	//20 threads create and run ThreadFunc function. function's argument is argument[i]
	
	for(i=0;i<ARGUMENT_NUMBER;i++)
	{
		pthread_create(&threadID[i],NULL,ThreadFunc,(void*)&argument[i]);
	}

	printf("Main Thread is waiting for Child Thread!\n");
	
	//wait for the end of the 20 threads
	for(i=0;i<ARGUMENT_NUMBER;i++)
	{
		pthread_join(threadID[i],NULL);
	}

	//variable to add result of each thread
	long long total=0;
	
	//add the result of each thread to the total variable
	for(i=0;i<ARGUMENT_NUMBER;i++)
	{
		total+=result[i];
	}

	printf("result = %lld\n",total);
	return 0;
}
