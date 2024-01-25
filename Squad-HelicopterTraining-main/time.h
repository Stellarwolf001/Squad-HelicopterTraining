#ifndef TIME__H
#define TIME__H

#include "./config.h"

#define TimeDifference 10

extern time_t currentTime;
extern struct tm *localTime;

// 定义结构体存储日期和时间
struct DateTime 
{
    char year[5];
    char month[3];
    char day[3];
    char hour[3];
    char minute[3];
    char second[3];
    char millisecond[4];
}logDateTime;  

//将日志中的月、日、时、分、秒组合成一个整数
unsigned int LogcombineDateTime_sec(char line[],struct DateTime logDateTime);

//将当前时间的月、日、时、分、秒组合成一个整数
unsigned int LocalcombineDateTime_sec(struct tm *localTime);

//判断时间
int IsTimeDifferenceWithinThreshold(unsigned int LogcombinedDateTime);

//更新时间
void time_updata(void);

#endif