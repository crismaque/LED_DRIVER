#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "user_app.h"

int main(int argc,char *argv[]){
	int fd;
	char buf[4];
				
	if(argc == 1){
		printf("Usage:\n %s on\n %s off\n %s status\n", argv[0], argv[0], argv[0]);
		return 1;
	}

	fd = open(DEVICE, O_RDWR);
		if(fd < 0){
		perror("Failed to open device");
		return 1;
	}

	if(strcmp(argv[1],"status") == 0){
		int r = read(fd,buf,sizeof(buf));
		if(r > 0){
			if(buf[0] == '1')
				printf("LED is ON\n");
			else if(buf[0] == '0')
				printf("LED is OFF\n");
			else
				printf("Unknown status\n");
		}else{
			printf("Failed to read status\n");
		}
	} else{
		write(fd,argv[1],strlen(argv[1]));
	}

	close(fd);
	return 0;
}

