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
void clear(){
	int i;
	for(i=0;i<25;i++){
	printf("\n");	
}

}


int rand()
{
    return (int) get_num();
}

int main(int argc, char * argv[])
{
	
	void draw(int a[][4]);
	void left(int a[][4]);
	void right(int a[][4]);
	void up(int a[][4]);
	void down(int a[][4]);
	void left_remove_blank(int a[][4]);
	void right_remove_blank(int a[][4]);
	void up_remove_blank(int a[][4]);
	void down_remove_blank(int a[][4]);
	void addNewCell(int a[][4]);
	char n[10] = {0};
	int a[4][4] = {{2,2,0,0},{0,4,0,0},{0,2,0,0},{0,0,4,0}};
	int i;
	
	draw(a);
    while(1){
    	scan(n);
        if(n[0]=='a'){//左   
        	left_remove_blank(a);
            left(a);
        }else if(n[0]=='d'){//右  
			right_remove_blank(a);
            right(a);  
        }else if(n[0]=='w'){//上 
			up_remove_blank(a);  
            up(a);
        } else if(n[0]=='s'){//下   
        	down_remove_blank(a);
            down(a);  
        }
		clear();
		addNewCell(a);  
        draw(a);
        if(n[0]=='q'){
        	return;
        }
    }
}



void addNewCell(int a[][4]) 
{
	int newCell,i,k,count = 0,newCount = 0,position = 0;
	
	//生成随机数 
    int randNum;
    randNum = rand();
	if(randNum%2 == 1) {
		newCell = 2;
    }
	else
		newCell = 4;
		
	for(i = 0;i < 4;i++) {
		for (k = 0;k < 4;k++) {
			if(a[i][k] == 0)
				count++;
		}
	}
	if(count == 0) {
        
    }
	//退出 
	
	position = rand()%count + 1;
	for(i = 0;i < 4;i++) {
		for (k = 0;k < 4;k++) {
			if(a[i][k] == 0)
				newCount++;
				
			if(newCount == position&&a[i][k] == 0)
				a[i][k] = newCell;
		}
	}
}
 
void draw(int a[][4]) 
{
	int i=0,j=0;
	printf("---------------------\n");
	for (i = 0;i < 3;i++) {
		for (j = 0;j < 4; j++)
		{
			if(a[i][j] == 0)
				printf("|    ");
			else
				printf("|%4d",a[i][j]);
		}
	printf("|\n");
	printf("|----|----|----|----|\n");
	}
	
	for (j = 0;j < 4; j++)
	{
		if(a[3][j] == 0)
			printf("|    ");
		else
			printf("|%4d",a[3][j]);
	}
	printf("|\n");
	printf("---------------------\n");
} 
 

void up_remove_blank(int a[][4]){  
    int i,j,k,temp;  
    for(j=0;j<4;j++){  
        for(i=1;i<4;i++){  
            k=i;  
            while(k-1>=0&&a[k-1][j]==0){//上面的那个为空  
				temp = a[k][j];
				a[k][j] = a[k-1][j];
				a[k-1][j] = temp;
                k--;  
            }  
        }  
    }  
}  
void down_remove_blank(int a[][4]){  
    int i,j,k,temp;  
    for(j=0;j<4;j++){  
        for(i=2;i>=0;i--){  
            k=i;
            while(k+1<=3&&a[k+1][j]==0){//上面的那个为空  
				temp = a[k][j];
				a[k][j] = a[k+1][j];
				a[k+1][j] = temp;
                k++;
            }
        }  
    }  
}  
void left_remove_blank(int a[][4]){  
    int i,j,k,temp;  
    for(i=0;i<4;i++){  
        for(j=1;j<4;j++){  
            k=j;  
            while(k-1>=0&&a[i][k-1]==0){//上面的那个为空   
            	temp = a[i][k];
				a[i][k] = a[i][k-1];
				a[i][k-1] = temp;
                k--;  
            }     
        }  
    }     
}   

void right_remove_blank(int a[][4]){  
    int i,j,k,temp;  
    for(i=0;i<4;i++){  
        for(j=2;j>=0;j--){  
            k=j;  
            while(k+1<=3&&a[i][k+1]==0){//上面的那个为空   
            	temp = a[i][k];
				a[i][k] = a[i][k+1];
				a[i][k+1] = temp; 
                k++;      
            }     
        }  
    }
}  

void left(int a[][4]){  
    int i,j;  
    for(i=0;i<4;i++){  
        for(j=0;j<3;j++){  
            if(a[i][j]==a[i][j+1]){  
                a[i][j]+=a[i][j+1];  
                a[i][j+1]=0;
                left_remove_blank(a);    
            }  
        }  
    }  
}  

void right(int a[][4]){  
    int i,j;  
    for(i=0;i<4;i++){  
        for(j=3;j>=1;j--){  
            if(a[i][j]==a[i][j-1]){  
                a[i][j]+=a[i][j-1];  
                a[i][j-1]=0;  
                right_remove_blank(a);    
            }  
        }  
    }  
} 

void up(int a[][4]){  
    int i,j;  
    for(j=0;j<4;j++){  
        for(i=0;i<3;i++){  
            if(a[i][j]==a[i+1][j]){  
                a[i][j]=a[i][j]+a[i+1][j];  
                a[i+1][j]=0;
                up_remove_blank(a);    
            }  
        }   
    }  
}  
void down(int a[][4]){  
    int i,j;  
    for(j=0;j<4;j++){   
        for(i=3;i>=1;i--){  
            if(a[i][j]==a[i-1][j]){  
                a[i][j]=a[i][j]+a[i-1][j];  
                a[i-1][j]=0; 
                down_remove_blank(a);    
            }  
        }   
    }  
}  

