#include "./command.h"

//定义命令字符串数组
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

//获取命令数组的长度
size_t numCommands = sizeof(commands) / sizeof(commands[0]);

//完整的命令字符串
char fullCommand[512];
//适当大小的缓冲区，用于保存命令输出
char commandOutput[1024];

int commandExecuted = 1;

int RCONCommand(const char *command, char *output)
{
    int result;
    do {
        // 执行指令
        result = RconSendCommand(command, output);

        // 如果输出中包含"Illegal size"，则重新执行指令
    } while (strstr(output, "Illegal size") || strstr(output, "recv() failed"));

    return 0; // 指令执行成功
}

//检查日志内容及时间
int CheckString(char *line, char *string)
{
    //检查是否包含特定子串
    if (strstr(line, string) != NULL) 
    {
        time_updata();
        return 1;
    }

    return 0;  // 返回0表示不包含特定子串
}

//获取游戏内人数
int GetGamePlayerNum()
{
    //执行指令并获取输出
    RCONCommand("ShowServerInfo", commandOutput);

    //查找"PlayerCount_I"字段
    const char *playerCountStart = strstr(commandOutput, "\"PlayerCount_I\":\"");
    if (playerCountStart != NULL)
    {
        //定位到数字的起始位置
        const char *countStart = playerCountStart + strlen("\"PlayerCount_I\":\"");
        
        //使用sscanf提取服务器内人数
        int playerCount;
        if (sscanf(countStart, "%d", &playerCount) == 1)
        {
            return playerCount;
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

//获取玩家昵称
char* GetGameLogString(const char *line, const char *frontString, const char *searchString)
{
    const char *playerInTurret = strstr(line, frontString);

    if (playerInTurret != NULL)
    {
        const char *countStart = playerInTurret + strlen(frontString);
        char playerName[256];

        if (sscanf(countStart, searchString, playerName) == 1)
        {
            char *result = strdup(playerName);

            return result;
        }
    }

    return NULL;
}

//执行指令跳过开局时间
void Skip_Time(struct tm *localTime,int command)
{
    // 打印执行指令的时间
    printf("[%d.%d.%d-%d:%d:%d]", localTime->tm_year + 1900, localTime->tm_mon + 1, localTime->tm_mday, localTime->tm_hour, localTime->tm_min, localTime->tm_sec);
    switch(command)
    {
        case CHANGELAYER:
            printf("ChangeLayer\n");break;
        case PLAYERJOIN:
            printf("SkipTime\n");break;
    }

    //延时5秒
    Delay_ms(5000);

    for (size_t i = 0; i < numCommands; i++)
    {
        RCONCommand(commands[i], commandOutput);
        //延时1秒
        Delay_ms(1000);
    }
}