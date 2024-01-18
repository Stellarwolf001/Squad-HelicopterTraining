/*此代码主要功能为：
 *识别日志中换图的时间戳，
 *与实际时间做对比，
 *差值小于15s及执行加速及取消限制指令
 *本人刚刚入门，代码风格较为浅显，大佬请见谅
 *
 *请将以下内容进行修改
 * 23行 *rconPath = "C:\\Users\\Administrator\\Desktop\\rcon.exe";
 * 24行 *serverPassword = "-PYourRCONPassword";
 * 25行 *serverPort = "-p21114";
 * 26行 *log_address = "C:\\Users\\Administrator\\Desktop\\squad_server\\SquadGame\\Saved\\Logs\\SquadGame.log";
 * 
 * Squad红警_Stellarwolf编写
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// 注意替换下面的路径为实际的 rcon.exe 文件路径
const char *rconPath = "C:\\Users\\Administrator\\Desktop\\rcon.exe";
const char *serverPassword = "-PYourRCONPassword";
const char *serverPort = "-p21114";
const char *log_address = "C:\\Users\\Administrator\\Desktop\\squad_server\\SquadGame\\Saved\\Logs\\SquadGame.log";

// 定义命令字符串数组
const char *commands[] = 
{
    "AdminSlomo 20",
    "AdminForceAllVehicleAvailability 1",
    "AdminForceAllRoleAvailability 1",
    "AdminForceAllDeployableAvailability 1",
    "AdminForceAllRoleAvailability 1",
    "AdminDisableVehicleTeamRequirement 1",
    "AdminDisableVehicleKitRequirement 1",
    "AdminDisableVehicleClaiming 1",
    "AdminNoRespawnTimer 1",
    "AdminSlomo 1"
};

//获取当前时间的时间戳
time_t currentTime;

//完整的命令字符串
char fullCommand[512];

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
unsigned int LogcombineDateTime_sec(char line[],struct DateTime logDateTime) 
{
    int result = 0,mouth,day,hour,minute,second;// 提取年、月、日、小时、分钟、秒
    sscanf(line, "[%4[^.].%2[^.].%2[^-]-%2[^.].%2[^.].%2[^:]:%2[^.].%2[^.]:%3[^]]", logDateTime.year, logDateTime.month, logDateTime.day, logDateTime.hour, logDateTime.minute, logDateTime.second, logDateTime.millisecond);
    mouth = strtol(logDateTime.month, NULL, 10) * 2592000;
    day = strtol(logDateTime.day, NULL, 10) * 86400;
    hour = strtol(logDateTime.hour, NULL, 10) * 3600;
    minute = strtol(logDateTime.minute, NULL, 10) * 60;
    second = strtol(logDateTime.second, NULL, 10);
    result = mouth + day + hour + minute + second + 28800;

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
    time(&currentTime);
    struct tm *localTime = localtime(&currentTime);
    
    unsigned int LocalcombinedDateTime = LocalcombineDateTime_sec(localTime);

    if( (LocalcombinedDateTime - LogcombinedDateTime) < 15 )
        return 1;
    else
        return 0;
}

int executeCommand(const char *command, char *output, size_t outputSize) 
{
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        perror("Error opening pipe");
        return -1;
    }

    // 读取命令输出
    size_t bytesRead = fread(output, 1, outputSize - 1, fp);
    if (bytesRead == 0) {
        perror("Error reading from pipe");
        pclose(fp);
        return -1;
    }

    output[bytesRead] = '\0';  // 添加字符串结束符

    // 关闭文件指针
    int status = pclose(fp);
    if (status == -1) {
        perror("Error closing pipe");
        return -1;
    }

    return 0;
}

//执行指令
int Skip_Time(struct tm *localTime)
{
    //打印执行指令的时间
    printf("Skip_Time\n");
    printf("localtime = [%d.%d.%d-%d:%d:%d]\n\n",localTime->tm_year + 1900,localTime->tm_mon + 1,localTime->tm_mday,localTime->tm_hour,localTime->tm_min,localTime->tm_sec);

    //等待5秒
    system("timeout /t 5 /nobreak >nul");

    //获取命令数组的长度
    size_t numCommands = sizeof(commands) / sizeof(commands[0]);

    for (size_t i = 0; i < numCommands; i++) 
    {
        snprintf(fullCommand, sizeof(fullCommand), "%s %s %s %s", rconPath, serverPort, serverPassword, commands[i]);

        //执行指令
        int result = system(fullCommand);

        //检查执行结果
        if (result != 0) 
        {
            fprintf(stderr, "Failed to execute command: %s\n", fullCommand);
            return 1;
        }

        //等待1秒
        system("timeout /t 1 /nobreak >nul");
    }
}

int main() 
{    
    FILE *file;
    char line[512];

    //获取当前时间的时间戳
    time(&currentTime);

    //使用 localtime 函数将时间戳转换为本地时间
    struct tm *localTime = localtime(&currentTime);

    printf("脚本启动成功，当前时间为：\n");
    printf("localtime = [%d.%d.%d-%d:%d:%d]\n\n",localTime->tm_year + 1900,localTime->tm_mon + 1,localTime->tm_mday,localTime->tm_hour,localTime->tm_min,localTime->tm_sec);

    while(1)
    {
        file = fopen(log_address, "r");

        if (file == NULL) 
        {
            perror("Error opening file");
            fclose(file);
            return 1;
        }

        while (fgets(line, sizeof(line), file)) 
        {
            // 检查是否包含关键字
            if(strstr(line, "StartLoadingDestination") != NULL) 
            {
                if(IsTimeDifferenceWithinThreshold(LogcombineDateTime_sec(line,logDateTime)))
                {   
                    Skip_Time(localTime);
                }
                else
                {
                    //等待1秒
                    system("timeout /t 1 /nobreak >nul");
                }
            }
            if(strstr(line, "Join succeeded") != NULL)
            {
                if(IsTimeDifferenceWithinThreshold(LogcombineDateTime_sec(line,logDateTime)))
                {
                    //向服务器发送ShowServerInfo指令获取服务器状态
                    snprintf(fullCommand, sizeof(fullCommand), "%s %s %s %s", rconPath, serverPort, serverPassword, "ShowServerInfo");

                    char commandOutput[1024]; // 适当大小的缓冲区，用于保存命令输出

                    //执行指令并获取输出
                    executeCommand(fullCommand, commandOutput, sizeof(commandOutput));

                    //查找"PlayerCount_I"字段
                    const char *playerCountStart = strstr(commandOutput, "\"PlayerCount_I\":\"");
                    if (playerCountStart != NULL) 
                    {
                        //定位到数字的起始位置
                        const char *countStart = playerCountStart + strlen("\"PlayerCount_I\":\"");
                        
                        //使用sscanf提取数字及服务器内人数
                        int playerCount;
                        if (sscanf(countStart, "%d", &playerCount) == 1) 
                        {
                            //服务器内仅有一人，执行指令
                            if(playerCount == 1)
                            {
                                Skip_Time(localTime);
                            }
                        } 
                        else 
                        {
                            printf("Failed to extract player count.\n");
                        }
                    } 
                    else 
                    {
                        printf("PlayerCount_I not found in the string.\n");
                    }
                }
                else
                {
                    //等待1秒
                    system("timeout /t 1 /nobreak >nul");
                }
            }
        }
        fclose(file);
    }

    return 0;
}