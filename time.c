#include "./mytime.h"
 
//获取当前时间的时间戳
time_t currentTime;
struct tm *localTime;

void time_updata()
{
    // 获取当前时间的时间戳
    time(&currentTime);
    localTime = localtime(&currentTime);
    #if !LINUXORWINDOWS
    // 计算当前时间加上八小时的秒数
    currentTime += 8*60*60;
    // 使用 localtime 函数将时间戳转换为本地时间
    localTime = localtime(&currentTime);
    #endif
}

//将当前时间的月、日、时、分、秒组合成一个整数
unsigned int LocalcombineDateTime_sec(struct tm *localTime) 
{
    unsigned int result;

    result = (localTime->tm_mon + 1) * 2592000+
    (localTime->tm_mday) * 86400+
    (localTime->tm_hour) * 3600+
    (localTime->tm_min) * 60+
    (localTime->tm_sec);

    return result;
}

void Delay_ms(int ms)
{
    #if LINUXORWINDOWS
    //延时（以毫秒为单位）
    Sleep(ms);
    #else    
    //延时（以微秒为单位）
    usleep(ms*1000);
    #endif
}