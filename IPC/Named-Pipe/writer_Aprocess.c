#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>

#define MSG_SIZE 200
#define PIPENAME "./named_pipe_file"

int main(void)
{
	char msg[MSG_SIZE];
	char string[200];
	int fd;
	int ret;

	fd = open(PIPENAME, O_WRONLY);
	if(fd<0)
	{
		printf("Open failed\n");
		return 0;
	}
	printf("Hello, this is A procrss. I'll give the data to B.\n");

	while(1)
	{
		fgets(string,sizeof(string),stdin);
		string[strlen(string)-1] = '\0';
		snprintf(msg,sizeof(msg),"%s",string);
		ret = write(fd, msg, sizeof(msg));
		if(ret<0)
		{
			printf("Write failed\n");
			return 0;
		}
	}
	return 0;
}

