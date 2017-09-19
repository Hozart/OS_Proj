#include "type.h"
#include "stdio.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "fs.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "proto.h"

void main(){
	char pw[128];
	for(int i = 0;i < 128;i++) {
		pw[i] = 0;
	}
	printf("Set the password: \n");
	scan(pw);
	int fd = -1;
	fd = open("account", O_RDWR);
	
	if(fd == -1) {
		return;
	}
	
	int length = 0;
	int n;
	
	for(int i = 0;i <128;i++) {
		if(pw[i] == 0) {
			pw[i+1]=0;
			length = length + 2;
			break;
		}
		length++;
	}
	
	n = write(fd, pw, length);
	
	close(fd);
}