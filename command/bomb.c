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


int count=10;
int flag=1;
int disbomb[10][10] = {0};   
int a[10][10] = {0};//显示数组


void clear(){
	int i;
	for(i=0;i<25;i++){
	printf("\n");	
}
}

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

int countbomb(int x,int y)
{
    int countbomb=0;
    int i,j;
    //当且仅当该方块存在且含有雷，countbomb++
    if((disbomb[x-1][y-1])&&(disbomb[x-1][y-1]==1)&&(x-1>0)&&(y-1>0)) 
                    countbomb+=1;
    if((disbomb[x][y-1])&&(disbomb[x][y-1]==1)&&(x-1>0)&&(y-1>0)) 
                    countbomb+=1;
    if((disbomb[x+1][y-1])&&(disbomb[x+1][y-1]==1)&&(x-1>0)&&(y-1>0)) 
                    countbomb+=1;
    if((disbomb[x-1][y])&&(disbomb[x-1][y]==1)&&(x-1>0)&&(y-1>0)) 
                    countbomb+=1;
    if((disbomb[x+1][y])&&(disbomb[x+1][y]==1)&&(x-1>0)&&(y-1>0)) 
                    countbomb+=1;
    if((disbomb[x-1][y+1])&&(disbomb[x-1][y+1]==1)&&(x-1>0)&&(y-1>0)) 
                    countbomb+=1;
    if((disbomb[x][y+1])&&(disbomb[x][y+1]==1)&&(x-1>0)&&(y-1>0)) 
                    countbomb+=1;
    if((disbomb[x+1][y+1])&&(disbomb[x+1][y+1]==1)&&(x-1>0)&&(y-1>0)) 
                    countbomb+=1;
	if(countbomb==0) {
		a[x][y]=9;
	}
	else{
		a[x][y]=countbomb;
	}
    return countbomb;
}

void makebomb()
{
//    srand((int)time(0));
    int x,y;
    //要取得[a,b]的随机整数，使用(rand() % (b-a+1))+ a
    int i = count;
    while(i>0)
    {
                x=(rand()%(9-1+1))+1;
                y=(rand()%(9-1+1))+1;
                if(disbomb[x][y]!=1) 
                {
                    disbomb[x][y]=1;
                    i--;
                }
        }    
}

//打印
void printbomb()
{
    int i,j,num = 0;
    for( i=0;    i<10;    i++)
        {    
            for( j=0;    j<10;    j++)
                {
				if (a[i][j] == 0)
				{
					num++;
				}
                printf("%d ",a[i][j]);
                }
            printf("\n");
        }
        
	if (num == 81 - count) {
		printf("WinnerWinner,ChickenDinner!");
		//TODO exit
		flag = -1;
	}
	
}

//游戏结束后将所有雷打印出来
void gameover()
{
    int i,j;
    for(i=0;    i<10;    i++)
    {
        for( j=0;    j<10;    j++)
        {
            printf("%d ",disbomb[i][j]);
        }
        printf("\n");
    }
}

int main()
{
    int X,Y;
    char x_char[10],y_char[10];
    int i,j;
    printf("***********************\n");
    printf("*******BombBomb********\n");
    printf("***********************\n");
    printf("If you didnt touch the bomb,\n");
	printf("it will show a num means the number of bombs nearby,\n");
	printf("and '9' means there is no bomb nearby\n");
	printf("-------------------------------------\n");
	for (i=0;i < 10;i++) {
		for (j=0;j < 10;j++) {
			a[i][j]=0;
			disbomb[i][j]=0;
		}
	}
	for (i=0;i < 10;i++) {
		a[i][0]=i;
		a[0][i]=i;
		disbomb[i][0]=i;
		disbomb[0][i]=i;
	}

    for( i=0;    i<10;    i++)
        {
            for( j=0;    j<10;    j++)
            {
                printf("%d ",disbomb[i][j]);
            }
            printf("\n");
        }
    makebomb();
    while(flag)
    {
    	
        printf("input x y with a ENTER between x&y\n");
        scan(x_char);
        scan(y_char);
        X = x_char[0] - '0';//change char to int 
        Y = y_char[0] - '0';
	if(X==0&Y==0)
{
return;
}
        if(X<1||X>9||X<1||X>9){
        	printf("x&y should be 1-9\n");
		}
	clear();
        if(disbomb[X][Y]==0)
        {
            countbomb(X,Y);
            printbomb();
        }
        else
        {
            printf("sorry,game over\n");
            gameover();
            flag=-1;
            return 0;

        }        
    }
    return 0;
}
