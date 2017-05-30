#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>

void pri_usage(char *info)
{
	printf("-------- usage: ------\n");
	printf("usage1: %s stat\n", info);
	printf("usage2: %s num on/off\n", info);
	printf("           num: <1~4>\n");
}

//a.out led stat
//./a.out led 1 on
int main(int argc, char **argv)
{
	int i;
	int ret;
	int num;
	char buf[4];
	int fd;

	unsigned char cmd = 0;

	if((argc !=  3) && (argc != 4)){
		pri_usage(argv[0]);
		exit(1);
	}

	fd = open(argv[1], O_RDWR | O_NONBLOCK);
	if(fd < 0){
		perror("open");
		exit(1);
	}

	if(argc == 3){
		if(!strncmp("stat", argv[2], 4)){
			ret = read(fd, buf, sizeof(buf));	
			if(ret != 4){
				perror("read");
				exit(1);
			}
			for(i = 0; i < sizeof(buf); i++){
				printf("led %d is %s\n", \
						i+1, buf[i]?"on":"off");
			}
		}else{
			pri_usage(argv[0]);
			exit(1);
		}
	}else{
		num = atoi(argv[2]);
		if(num >= 1 && num <= 4){
			cmd &= ~(0xf << 4);
			cmd = num << 4;
		}else{
			pri_usage(argv[0]);
			exit(1);
		}

		if(!strncmp("on", argv[3], 2)){
			cmd &= ~0xf;
			cmd |= 1;
		}else if(!strncmp("off", argv[3], 3)){
			cmd &= ~0xf;
		}else{
			pri_usage(argv[0]);
			exit(1);
		}
		ret = write(fd, &cmd,sizeof(cmd));
		if(ret != sizeof(cmd)){
			pri_usage(argv[0]);
			exit(1);
		}
	}

	return 0;
}
