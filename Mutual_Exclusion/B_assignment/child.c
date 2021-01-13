#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>

#define COUNTING_NUMBER 2000000

//structure corresponding to shared memory. name is smStruct.
typedef struct
{
	int critical_section_variable;
	int turn;
	int flag[2];

}smStruct;

//change the flag in shared memory smstruct corresponding to the valued received by argument(order) to 1 and change the turn to other, and wait in while section until the other is out of the critical section 
void lock(smStruct *smstruct, int order)
{
	smstruct->flag[order] = 1;
	smstruct->turn = 1-order;
	while(smstruct->flag[1-order]==1 && smstruct->turn==1-order);
}
//change the flag in shared memory smstruct to zero because the order process is out of the critical section
void unlock(smStruct *smstruct, int order)
{
	smstruct->flag[order]=0;
}

int main(void)
{
//variables that actually count the behavior that increases the critical_section_value
	int localcount =0;
	
	smStruct *smstruct = NULL;

	int shmid;
	void *shmaddr;
	int ret;
//get shared memory id or create		
	shmid = shmget((key_t)1234,1024,IPC_CREAT|0666);
	if (shmid ==-1)
	{
		perror("shmget failed\n");
		exit(0);
	}
//attach shared memory to process memory space
	shmaddr = shmat(shmid,(void*)0,0);
	
	smstruct = shmaddr;
//Variable used by lock and unlock after obtaining information about process id.this variable has a value of 0 or 1 because two process occur consecutively.

	int Myorder = getpid()%2;	

//allow only one thread at a time to increase the critical_section_variable to COUNTING_NUMBER	
	for(int i=0;i<COUNTING_NUMBER;i++)
	{
		localcount++;
		lock(smstruct, Myorder);
		smstruct->critical_section_variable++;
		unlock(smstruct,Myorder);
	}
	
	printf("Myorder = %d,process pid = %d\n", Myorder, getpid());
	printf("child finish! local count = %d\n", localcount);

//detach shared memory
	ret = shmdt(shmaddr);
	if(ret == -1)
	{
		perror("detach failed\n");
		return 0;
	}

	return 0;

}
