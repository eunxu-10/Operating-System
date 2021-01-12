#include<stdio.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<string.h>

#define READ_WRITER_FLAG 0
#define READ_READER_FLAG 1

int main(void)
{
	int shmid;
	char *shmaddr;
	char *string;
	int ret;

	shmid =shmget((key_t)1234,1024,IPC_CREAT|0666);
	if(shmid<0)
	{
		perror("shmget");
		return 0;
	}

	shmaddr = shmat(shmid, (void *)0,0);
    if(shmaddr == (char *)-1)
    {
    	perror("attach failed\n");
        return 0;
    }

	shmaddr[0] = READ_WRITER_FLAG;

	string = shmaddr +1;

	while(1){
		
		if(shmaddr[0] == READ_WRITER_FLAG)
		{
			fgets(string, 200, stdin);
			string[strlen(string)-1]='\0';

			shmaddr[0]=READ_READER_FLAG;
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

	return 0;
}
