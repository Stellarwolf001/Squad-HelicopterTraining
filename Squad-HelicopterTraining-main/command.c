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

int executeCommand(const char *command, char *output)
{
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        perror("Error opening pipe");
        return -1;
    }

    // 读取命令输出
    size_t bytesRead = fread(output, 1, COMMANDOUTPUT_SIZE - 1, fp);

    if (bytesRead < COMMANDOUTPUT_SIZE - 1) 
    {
        output[bytesRead] = '\0';  // 添加字符串结束符
    }
    else 
    {
        output[COMMANDOUTPUT_SIZE - 1] = '\0';  // 添加字符串结束符
    }

    // 关闭文件指针
    int status = pclose(fp);
    if (status == -1) {
        perror("Error closing pipe");
        return -1;
    }

    return 0;
}

int RCONCommand(const char *command, char *output)
{
    snprintf(fullCommand, sizeof(fullCommand), "%s %s %s %s", rconPath, serverPort, serverPassword, command);

    //执行指令
    int result = executeCommand(fullCommand, output);

    //检查执行结果
    if (result == -1 || WEXITSTATUS(result) != 0)
    {
        fprintf(stderr, "Failed to execute command: %s\n", fullCommand);
        return 1;
    }
}

//检查日志内容及时间
int CheckstringAndTime(char *line, char *string)
{
    //检查是否包含特定子串
    if (strstr(line, string) != NULL) 
    {
        // 在这里执行其他操作
        if (IsTimeDifferenceWithinThreshold(LogcombineDateTime_sec(line, logDateTime)))
        {
            return 1;
        }
        else
        {
            usleep(1000);
        }
        return 0;  // 返回1表示包含特定子串
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
char* GetGamePlayerName(const char *line, const char *frontString, const char *searchString) 
{
    const char *playerInTurret = strstr(line, frontString);

    if (playerInTurret != NULL) {
        const char *countStart = playerInTurret + strlen(frontString);
        char playerName[256];

        if (sscanf(countStart, searchString, playerName) == 1) {
            // 动态分配内存
            char *result = strdup(playerName);

            // 返回新分配的内存
            return result;
        }
    }

    return NULL;
}

//执行指令跳过开局时间
void Skip_Time(struct tm *localTime)
{
    // 打印执行指令的时间
    printf("[%d.%d.%d-%d:%d:%d]", localTime->tm_year + 1900, localTime->tm_mon + 1, localTime->tm_mday, localTime->tm_hour, localTime->tm_min, localTime->tm_sec);
    printf("ChangeLayer\n");

    //延时5秒（以微秒为单位）
    usleep(5000000);

    for (size_t i = 0; i < numCommands; i++)
    {
        RCONCommand(commands[i], commandOutput);
        //延时1秒（以微秒为单位）
        usleep(1000000);
    }
}