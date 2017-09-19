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

PUBLIC int get_num()
{
    MESSAGE msg;
    memset(&msg, 0, sizeof(MESSAGE));
    msg.type = GET_TICKS;
    send_recv(BOTH, TASK_SYS, &msg);
    return msg.RETVAL;
}

int rand()
{
    return (int) get_num();
}

int main(int argc, char * argv[]){
	while(1){
		char a[1];
		scan(a);

		if(a[0] == 'q')
			return 0;

		printf("%d\n", rand());
	}
}