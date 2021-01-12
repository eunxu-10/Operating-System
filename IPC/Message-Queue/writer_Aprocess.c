#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/stat.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>

struct msgbuf
{
	long msgtype;
	char mtext[200];
};

int main(void)
{
	key_t key_id;
	char string[200];
	int i=0;
	struct msgbuf sndbuf;

	key_id = msgget((key_t)1234,IPC_CREAT|0666);
	
	if(key_id == -1)
	{
		perror("msgget error : ");
		return 0;
	}

	sndbuf.msgtype =3;	
	while(1)
	{

		fgets(string, sizeof(string), stdin);
		string[strlen(string)-1] = '\0';
		strcpy(sndbuf.mtext, string);


		if(msgsnd(key_id, (void*)&sndbuf,sizeof(struct msgbuf),IPC_NOWAIT)==-1)
		{
			perror("msgsne error : ");
		}
		else
		{
			printf("Sending %dth message is succeed\n",i);
			i++;
		}
	}

	return 0;
}


