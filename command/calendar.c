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

#define Mon   1
#define Tues  2
#define Wed   3
#define Thur  4
#define Fri   5
#define Sat   6
#define Sun   0

#define January_days   31
#define February_days   28
#define March_days    31
#define April_days    30
#define May_days     31
#define June_days     30
#define July_days     31
#define August_days    31
#define September_days  30
#define October_days   31
#define November_days   30
#define December_days   31

#define first1month January_days
#define first2month January_days+February_days
#define first3month January_days+February_days+March_days
#define first4month January_days+February_days+March_days+April_days
#define first5month January_days+February_days+March_days+April_days+May_days
#define first6month January_days+February_days+March_days+April_days+May_days+June_days
#define first7month January_days+February_days+March_days+April_days+May_days+June_days \
	         +July_days
#define first8month January_days+February_days+March_days+April_days+May_days+June_days \
	         +July_days+August_days
#define first9month January_days+February_days+March_days+April_days+May_days+June_days \
	         +July_days+August_days+September_days
#define first10month January_days+February_days+March_days+April_days+May_days+June_days \
	         +July_days+August_days+September_days+October_days
#define first11month January_days+February_days+March_days+April_days+May_days+June_days \
	         +July_days+August_days+September_days+October_days+November_days

int known_weekday = Tues;
int known_year = 1901;
int konwn_month = 1;
int known_day = 1;

int day_count(int month)
{	
	switch(month)
	{
		case 1: return 0;break;
		case 2: return first1month;break;
		case 3: return first2month;break;
		case 4: return first3month;break;
		case 5: return first4month;break;
		case 6: return first5month;break;
		case 7: return first6month;break;
		case 8: return first7month;break;
		case 9: return first8month;break;
		case 10: return first9month;break;
		case 11: return first10month;break;
		case 12: return first11month;break;
	}
}


char * month_name(int month)
{
	switch(month)
	{
		case 1:
			return "January";
			break;
		case 2:
			return "February";
			break;
		case 3:
			return "March";
			break;
		case 4:
			return "April";
			break;
		case 5:
			return "May";
			break;
		case 6:
			return "June";
			break;
		case 7:
			return "July";
			break;
		case 8:
			return "August";
			break;
		case 9:
			return "September";
			break;
		case 10:
			return "October";
			break;
		case 11:
			return "November";
			break;
		case 12:
			return "December";
			break;
		default:
			break;
	}
}
//
//char * month_name(int month)
//{
//	switch(month)
//	{
//		case 1:
//			return "一月";
//			break;
//		case 2:
//			return "二月";
//			break;
//		case 3:
//			return "三月";
//			break;
//		case 4:
//			return "四月";
//			break;
//		case 5:
//			return "五月";
//			break;
//		case 6:
//			return "六月";
//			break;
//		case 7:
//			return "七月";
//			break;
//		case 8:
//			return "八月";
//			break;
//		case 9:
//			return "九月";
//			break;
//		case 10:
//			return "十月";
//			break;
//		case 11:
//			return "十一月";
//			break;
//		case 12:
//			return "十二月";
//			break;
//		default:
//			break;
//	}
//}

/*按格式打印某年某月名称*/
void first_line_print(int month, int year)
{
	//printf("%20s %d \n",month_name(month),year);
	printf("%8dyear %s \n",year, month_name(month));
}

/*按格式打印星期名称*/
void week_print()
{
	//printf("%-6s%-6s%-6s%-6s%-6s%-6s%-6s\n","Sun.", "Mon.", "Tues.", "Wed.", "Thur.", "Fri.", "Sat.");
	printf("%3s%3s%3s%3s%3s%3s%3s\n","7 ", "1 ", "2 ", "3 ", "4 ", "5 ", "6 ");
	printf("--------------------\n");
}

/*计算该年该月份与已知日子之间的距离天数*/
int date_distance_count(int month, int year)
{
	int leap_year_count = 0;
	int i;
	int distance;

	if (year > known_year)
	{
		for (i=known_year; i<year; i++)
		{
			if(((i%4 == 0) && (i%100 != 0) ) || (i%400 == 0))
			{
				leap_year_count++;		
			}
		}

		if (month > 2)
		{
			if(((year%4 == 0) && (year%100 != 0) ) || (year%400 == 0))
			{
				leap_year_count++;		
			}
		}
	}
	else
	if (year == known_year)
	{
		if (month > 2)
		{
			leap_year_count = 1;
		}
	}
	

	distance = (year - known_year)*365 + leap_year_count + day_count(month);

	return distance;	
}

/*确定该月份第一天是星期几*/
int makesure_firstday_weekday(int month, int year)
{
	int date_distance = 0;
	int weekday;

	date_distance = date_distance_count(month, year);
	weekday = (known_weekday + date_distance)%7;

	return weekday;	
}

/*依次打印出该月份的日子*/
void print_in_turn(int month, int firstday, int year)
{
	int i = 1;
	int weekday;

	switch(firstday)
	{
		case Sun:
			break;
		case Mon:
			printf("   ");
			break;
		case Tues:
			printf("      ");
			break;
		case Wed:
			printf("         ");
			break;
		case Thur:
			printf("            ");
			break;
		case Fri:
			printf("               ");
			break;
		case Sat:
			printf("                  ");
			break;
	}

	switch(month)
	{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
		{
			for(i=0; i<31; i++)
			{
				weekday = (firstday + i)%7;
				printf("%3d",i+1);
				
				if(weekday == Sat)
				{
					printf("\n");
				}					
			}
			break;
		}
		case 2:
		{
			if(((year%4 == 0) && (year%100 != 0) ) || (year%400 == 0)) /*闰年*/
			{
				for(i=0; i<29; i++)
				{
					weekday = (firstday + i)%7;
					printf("%3d",i+1);
					
					if(weekday == Sat)
					{
						printf("\n");
					}					
				}
				break;
			}
			else /*平年*/
			{
				for(i=0; i<28; i++)
				{
					weekday = (firstday + i)%7;
					printf("%3d",i+1);
					
					if(weekday == Sat)
					{
						printf("\n");
					}					
				}
				break;
			}
		
		}
		case 4:
		case 6:
		case 9:
		case 11:
		{
			for(i=0; i<30; i++)
			{
				weekday = (firstday + i)%7;
				printf("%3d",i+1);
				
				if(weekday == Sat)
				{
					printf("\n");
				}					
			}
			break;
		}			
	}
}

void date_print(int month, int year)
{
	int firstday;

	firstday = makesure_firstday_weekday(month, year);
	print_in_turn(month, firstday, year);
	printf("\n");
}

void main_month(int month, int year)
{
	first_line_print(month, year);
	week_print();
	date_print(month, year);
	printf("\n\n");
}

void main_calendar(int year)
{
	int i;
	for(i=1; i<=12; i++)
	{
		main_month(i, year);
	}
}

int main()
{
	while(1)
	{
		char year_char[10];
		int year;
		printf("input '9999' means exit\n");
		printf("Please Input The Year--->year = ");
		scan(year_char);
		year = (year_char[0] - '0')*1000 + (year_char[1] - '0')*100 + (year_char[2] - '0')*10 + (year_char[3] - '0')*1;
		printf("\n");
		if (year == 9999){
			return;
		}
		while(year < 1902)
		{
			printf("the year should be > 1901\n");
	
			printf("Please Input The Year--->year = ");
			scan(year_char);
			year = (year_char[0] - '0')*1000 + (year_char[1] - '0')*100 + (year_char[2] - '0')*10 + (year_char[3] - '0')*1;
			printf("\n");
		}
	
		main_calendar(year);
	}
	return 0;
}
