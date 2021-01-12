#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<string.h>
#include<unistd.h>

#define READ_WRITER_FLAG 0
#define READ_READER_FLAG 1

int main(void)
{
	int shmid;
	char *shmaddr;
	int ret;
	char *string;

	shmid = shmget((key_t)1234,1024,IPC_CREAT|0666);
	if(shmid == -1)
	{
		perror("shared memory access is failed\n");
		return 0;
	}

	shmaddr = shmat(shmid, (void *)0,0);
	if(shmaddr ==(char *)-1)
	{

		perror("attach failed\n");
		return 0;
	}

	
	string = shmaddr +1;
	
	shmaddr[0] = READ_WRITER_FLAG;

	while(1)
	{
		if(shmaddr[0]==READ_READER_FLAG)
		{

			printf("data read from shared memory : %s\n", string);
			shmaddr[0]=READ_WRITER_FLAG;
			if(!strcmp(string, "quit"))
			{
				break;
			}
		}
	

	}

	ret = shmdt(shmaddr);
    if(ret == -1)
	{
    	perror("detach failed\n");
        return 0;
    }

    ret= shmctl(shmid, IPC_RMID,0);
    if(ret ==-1)
    {
        perror("remove failed\n");
        return 0;
    }

	return 0;
}

