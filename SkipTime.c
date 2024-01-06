/*此代码主要功能为：
 *识别日志中换图的时间戳，
 *与实际时间做对比，
 *差值小于15s及执行加速及取消限制指令
 *本人刚刚入门，代码风格较为浅显，大佬勿喷
 *
 *请将以下内容进行修改
 * 49行 *rconPath = "C:\\Users\\Administrator\\Desktop\\rcon.exe";
 * 50行 *serverPassword = "-PYourRCONPassword";
 * 51行 *serverPort = "-p21114";
 * 75行 file = fopen("C:\\Users\\Administrator\\Desktop\\squad_server\\SquadGame\\Saved\\Logs\\SquadGame.log", "r");
 * 
 * Squad红警_Stellarwolf编写
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
};

// 将日志中的月、日、时、分、秒组合成一个整数
int combineDateTime(struct DateTime logDateTime) 
{
    int result = 0,mouth,day,hour,minute,second;
    mouth = strtol(logDateTime.month, NULL, 10) * 2592000;
    day = strtol(logDateTime.day, NULL, 10) * 86400;
    hour = strtol(logDateTime.hour, NULL, 10) * 3600;
    minute = strtol(logDateTime.minute, NULL, 10) * 60;
    second = strtol(logDateTime.second, NULL, 10);
    result = mouth + day + hour + minute + second + 28800;

    return result;
}

int main() 
{    
    // 注意替换下面的路径为实际的 rcon.exe 文件路径
    const char *rconPath = "C:\\Users\\Administrator\\Desktop\\rcon.exe";
    const char *serverPassword = "-PYourRCONPassword";
    const char *serverPort = "-p21114";

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



    FILE *file;
    char line[512];

    while(1)
    {
        file = fopen("C:\\Users\\Administrator\\Desktop\\squad_server\\SquadGame\\Saved\\Logs\\SquadGame.log", "r");

        if (file == NULL) 
        {
            perror("Error opening file");
            return 1;
        }

        // 创建结构体变量存储日期和时间
        struct DateTime logDateTime;

        while (fgets(line, sizeof(line), file)) 
        {
            // 检查是否包含关键字
            if (strstr(line, "StartLoadingDestination") != NULL) 
            {
                // 提取年、月、日、小时、分钟、秒
                sscanf(line, "[%4[^.].%2[^.].%2[^-]-%2[^.].%2[^.].%2[^:]:%2[^.].%2[^.]:%3[^]]", logDateTime.year, logDateTime.month, logDateTime.day, logDateTime.hour, logDateTime.minute, logDateTime.second, logDateTime.millisecond);

                // 将年、月、日、小时、分钟、秒组合成一个整数
                int LogcombinedDateTime = combineDateTime(logDateTime);    

                // 获取当前时间的时间戳
                time_t currentTime;
                time(&currentTime);

                // 使用 localtime 函数将时间戳转换为本地时间
                struct tm *localTime = localtime(&currentTime);

                int LocalcombinedDateTime = (localTime->tm_mon + 1) * 2592000+
                (localTime->tm_mday) * 86400+
                (localTime->tm_hour) * 3600+
                (localTime->tm_min) * 60+
                (localTime->tm_sec);

                if( (LocalcombinedDateTime - LogcombinedDateTime) < 15 )
                {   
                    printf("Skip_Time\n");

                    // 等待5秒
                    system("timeout /t 5 /nobreak >nul");

                    // 获取命令数组的长度
                    size_t numCommands = sizeof(commands) / sizeof(commands[0]);

                    for (size_t i = 0; i < numCommands; ++i) 
                    {
                        // 构建完整的命令字符串
                        char fullCommand[512];
                        snprintf(fullCommand, sizeof(fullCommand), "%s %s %s %s", rconPath, serverPort, serverPassword, commands[i]);

                        // 执行命令
                        int result = system(fullCommand);

                        // 检查执行结果
                        if (result != 0) 
                        {
                            fprintf(stderr, "Failed to execute command: %s\n", fullCommand);
                            return 1;
                        }

                        // 等待1秒
                        system("timeout /t 1 /nobreak >nul");
                    }
                }
                else
                {
                    // 等待1秒
                    system("timeout /t 1 /nobreak >nul");
                }
            }
        }
        line[0] = '\0';
    }

    fclose(file);
    return 0;
}


