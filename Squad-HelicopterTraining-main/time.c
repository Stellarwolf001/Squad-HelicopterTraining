#include "./time.h"
 

//获取当前时间的时间戳
time_t currentTime;
struct tm *localTime;

void time_updata()
{
    // 获取当前时间的时间戳
    time(&currentTime);
    localTime = localtime(&currentTime);    
    // 获取当前时间的时间戳
    time(&currentTime);

    // 计算当前时间加上八小时的秒数
    currentTime += 8*60*60;

    // 使用 localtime 函数将时间戳转换为本地时间
    localTime = localtime(&currentTime);
}

//将日志中的月、日、时、分、秒组合成一个整数
unsigned int LogcombineDateTime_sec(char line[],struct DateTime logDateTime) 
{
    int result = 0, month, day, hour, minute, second;// 提取年、月、日、小时、分钟、秒
    if (sscanf(line, "[%4[^.].%2[^.].%2[^-]-%2[^.].%2[^.].%2[^:]:%2[^.].%2[^.]:%3[^]]", logDateTime.year, logDateTime.month, logDateTime.day, logDateTime.hour, logDateTime.minute, logDateTime.second, logDateTime.millisecond) != 7) 
    {
        // 处理解析失败的情况
        printf("Error parsing timestamp in line: %s\n", line);
        return -1; // 或者你可以选择其他适当的处理方式
    }
    month = strtol(logDateTime.month, NULL, 10) * 2592000;
    day = strtol(logDateTime.day, NULL, 10) * 86400;
    hour = strtol(logDateTime.hour, NULL, 10) * 3600;
    minute = strtol(logDateTime.minute, NULL, 10) * 60;
    second = strtol(logDateTime.second, NULL, 10);
    result = month + day + hour + minute + second + 8*60*60;

    return result;
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

//判断时间
int IsTimeDifferenceWithinThreshold(unsigned int LogcombinedDateTime)
{
    //更新当前时间
    time_updata();
    
    unsigned int LocalcombinedDateTime = LocalcombineDateTime_sec(localTime);

    if( (LocalcombinedDateTime - LogcombinedDateTime) < TimeDifference )
        return 1;
    else
        return 0;
}