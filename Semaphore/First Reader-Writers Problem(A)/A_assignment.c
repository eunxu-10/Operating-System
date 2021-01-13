#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>
#include<unistd.h>

#define COUNTING_NUMBER 10000
void reader();
void writer(void* arg);

sem_t S;
sem_t wrt;

int writer_access_count[5];
int readcount = 0;
int cur_writer;
int cur_count; 

int main(void)
{
	
	sem_init(&S, 0,1);
	sem_init(&wrt, 0,1);

	pthread_t r0,r1;
	pthread_t w0,w1,w2,w3,w4;
	int parameter[5] = {0,1,2,3,4};

//reader thread create
	pthread_create(&r0, NULL, (void*)&reader, NULL);
	pthread_create(&r1,NULL, (void*)&reader, NULL);

//writer thread create
	pthread_create(&w0, NULL, (void*)&writer, (void*)&parameter[0]);
	pthread_create(&w1, NULL, (void*)&writer, (void*)&parameter[1]);
	pthread_create(&w2, NULL, (void*)&writer, (void*)&parameter[2]);
	pthread_create(&w3, NULL, (void*)&writer, (void*)&parameter[3]);
	pthread_create(&w4, NULL, (void*)&writer, (void*)&parameter[4]);

//wait for the end of the thread
	pthread_join(r0, NULL);
	pthread_join(r1, NULL);

	pthread_join(w0, NULL);
	pthread_join(w1, NULL);
	pthread_join(w2, NULL);
	pthread_join(w3, NULL);
	pthread_join(w4, NULL);

//destory used semaphore
	sem_destroy(&S);
	sem_destroy(&wrt);
}

void writer(void *arg)
{
	int* writer_id= (int *)arg;
	int writer_iid = *writer_id;
	for(int i=0;i<COUNTING_NUMBER;i++)
	{
		usleep(100000);
		sem_wait(&wrt);
		//Assign thread discrimination factors writer_iid to the cur_writer
		cur_writer = writer_iid;
		//increase the writer_access_count array that indicate how many approaches the writer_iid has and assign that array to cur_count
		cur_count = writer_access_count[writer_iid]++;
		sem_post(&wrt);
	}
}

void reader( )
{

	int i;
	for(i =0 ;i<COUNTING_NUMBER;i++)
	{
		usleep(30000);
		//preventing race condition for readcount
		sem_wait(&S);
		readcount++;
		//if reader come to critical section, mutual exclusion of access between reader and writer
		if(readcount == 1){sem_wait(&wrt);}
		//increase semaphore S value since work on readcount is finished.
		sem_post(&S);
		printf("The most recent writer id : [%d], count: [%d]\n",cur_writer, cur_count);
		//preventing race condition for readcount
		sem_wait(&S); 
		readcount--;
		//if there's no one in the critical section, increase semaphore wrt
		if(readcount==0){sem_post(&wrt);}
		//increase semaphore S value since work on readcount is finished
		sem_post(&S);

	}
}


