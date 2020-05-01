#include<stdio.h>
#include<errno.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#define MSG_SIZE 300

int main() {
	int fd,status;
	char dmsg[MSG_SIZE];  /*Buffer for read*/

	//**Open the dev_List device **//
	fd = open("/dev/dev_list",O_RDONLY);
	if(fd<0) {
		perror("ERROR to open");
		return 0;
	}

	//** read=0, then read loop done termination condition**//
	while((status = read(fd,dmsg,strlen(dmsg)))  > 0) {
		printf("%s\n",dmsg);
		memset(dmsg,0,sizeof(char)*MSG_SIZE);
	}
	if(status < 0) {
		perror("ERROR to read");
		return 0;
	}
	printf("\n");
	close(fd);

	return 0;
}


