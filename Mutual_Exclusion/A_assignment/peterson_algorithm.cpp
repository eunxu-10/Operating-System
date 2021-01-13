#include <atomic>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define COUNTING_NUMBER 2000000

using namespace std;

int critical_section_variable = 0;

atomic_int turn(0);
atomic_int flag0(0), flag1(0);

//change the flag corresponding to the valued received by the argument(self) to 1 and change the turn to other, and wait in while section until the other is out of the critical section 
void lock(int self)
{
	if(self ==0)
	{
		flag0 = 1;
		turn = 1;
		while(flag1 == 1 && turn == 1);
	}
	else
	{
		flag1 = 1;
		turn = 0;
		while(flag0==1 && turn == 0);
	}
}
//change flag to zero 
void unlock(int self)
{
	if(self ==0)
	{
		flag0 =0;
	}
	else
	{
		flag1 =0;
	}

}
void* func(void *s)
{
//Allocate the factor of the thread as thread_num
	int* thread_num = (int *)s;
	int i;
//increase critical_section_variable while allowing access to only one thread at a time	
	for (i=0;i<COUNTING_NUMBER;i++)
	{
		lock(*thread_num);
		critical_section_variable++;
		unlock(*thread_num);
	}
}

int main(void)
{
	pthread_t p1, p2;
	int parameter[2] = {0,1};

//create thread, execute the func function, and put a parameter in the argument of the function
	pthread_create(&p1,NULL, func, (void*)&parameter[0]);
	pthread_create(&p2,NULL, func, (void*)&parameter[1]);
//wait for the thread to be finished
	pthread_join(p1,NULL);
	pthread_join(p2, NULL);

	printf("Actual Count: %d | Expected Count: %d\n", critical_section_variable,COUNTING_NUMBER*2);

	return 0;
}
