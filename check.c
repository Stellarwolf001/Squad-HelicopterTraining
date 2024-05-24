#include "./check.h"
#include "./file.h"

int ChangeLayerPlayerNum = 1;
CheckChangeLayerTime CheckPlaynameAndTime[80];

void CheckStarkGame(char *line)
{
    if (CheckString(line, "StartLoadingDestination"))
    {
        Skip_Time(localTime, CHANGELAYER);
    }
}

void CheckOverGame(char *line)
{
    if (CheckString(line, "DetermineMatchWinner"))
    {
        RCONCommand("AdminSlomo 20", commandOutput);
    }
}

void CheckPlayerJoin(char *line)
{
    if (CheckString(line, "Join succeeded"))
    {
        char *PlayerName = GetGameLogString(line, "LogNet: ", "Join succeeded: %[^\n]");

        printf("[%d.%d.%d-%d:%d:%d]", localTime->tm_year + 1900, localTime->tm_mon + 1, localTime->tm_mday, localTime->tm_hour, localTime->tm_min, localTime->tm_sec);
        printf("PlayerJoin: %s\n", PlayerName);

        if (GetGamePlayerNum() == 1)
        {
            Skip_Time(localTime, PLAYERJOIN);
        }

        // 释放内存
        free(PlayerName);
    }
}

// 检测是否有玩家造成恶意伤害
void CheckDamage(char *line)
{
    if (CheckString(line, "Player Controller") && CheckNonHelicopter(line))
    {
        char *KillerPlayerName = GetGameLogString(line, "from ", "%[^(] (Online");
        char *VictimPlayerName = GetGameLogString(line, "LogSquad: Player:", "%[^A] ActualDamage");

        float ActualDamage;
        sscanf(line, "%*[^=]=%f", &ActualDamage);

        if (KillerPlayerName != NULL && VictimPlayerName != NULL && strcmp(KillerPlayerName, VictimPlayerName) != 0 && strcmp(KillerPlayerName, "nullptr") != 0 && strcmp(VictimPlayerName, "nullptr") != 0)
        {
            char adminCommand[512];
#if CHECKDAMAGE
            printf("[%d.%d.%d-%d:%d:%d]", localTime->tm_year + 1900, localTime->tm_mon + 1, localTime->tm_mday, localTime->tm_hour, localTime->tm_min, localTime->tm_sec);
            printf("Was kicked for a kill:Player %s\n", KillerPlayerName);

            snprintf(adminCommand, sizeof(adminCommand), "AdminKick %s 直升机训练场请勿对玩家造成伤害", KillerPlayerName);
            RCONCommand(adminCommand, commandOutput);
            snprintf(adminCommand, sizeof(adminCommand), "AdminBroadcast %s 对%s造成伤害,已被自动踢出", KillerPlayerName, VictimPlayerName);
            RCONCommand(adminCommand, commandOutput);
#endif
#if CHECKDAMAGEWARNING
            snprintf(adminCommand, sizeof(adminCommand), "AdminWarn %s 你被%s造成了（%f伤害）", VictimPlayerName, KillerPlayerName, ActualDamage);
            RCONCommand(adminCommand, commandOutput);
#endif
        }

        // 释放内存
        free(KillerPlayerName);
        free(VictimPlayerName);
    }
}

// 检测是否有玩家进入炮手位
void CheckPlayerInTurret(char *line, char *vehicleType, char *seatNumber)
{
    if (CheckString(line, vehicleType) && CheckString(line, seatNumber) && CheckString(line, "OnPossess()") && CheckNonHelicopter(line))
    {
        char *PlayerName = GetGameLogString(line, "OnPossess(): ", "PC=%[^ ] (Online");

        printf("[%d.%d.%d-%d:%d:%d]", localTime->tm_year + 1900, localTime->tm_mon + 1, localTime->tm_mday, localTime->tm_hour, localTime->tm_min, localTime->tm_sec);
        printf("Was kicked for entering the Turret:Player %s\n", PlayerName);

        char adminCommand[512];
        snprintf(adminCommand, sizeof(adminCommand), "AdminKick %s 直升机训练场请勿进入固定武器或载具炮手位！！！", PlayerName);
        RCONCommand(adminCommand, commandOutput);
        snprintf(adminCommand, sizeof(adminCommand), "AdminBroadcast %s 已被自动踢出，直升机训练场请勿进入固定武器或载具炮手位！！！", PlayerName);
        RCONCommand(adminCommand, commandOutput);

        // 释放内存
        free(PlayerName);
    }
}

// 检测字符串不包含直升机
int CheckNonHelicopter(const char *line)
{
    if (!strstr(line, "UH60") && !strstr(line, "MI8") && !strstr(line, "UH1Y") &&
        !strstr(line, "Z8G") && !strstr(line, "MRH90") && !strstr(line, "MI17") &&
        !strstr(line, "CH146") && !strstr(line, "SA330") && !strstr(line, "Z8J") &&
        !strstr(line, "UH1H"))
        return 1;
    else
        return 0;
}

void CheckPlayerInAllTurret(char *line)
{
#if CHECKPLAYERINTURRET
    CheckPlayerInTurret(line, "Turret", "Seat Number");    // 检测大多数载具，包括但不限于：侦察车，坦克炮手，飞机机枪
    CheckPlayerInTurret(line, "turret", "Seat Number");    // 检测大多数载具，包括但不限于：侦察车，坦克炮手，飞机机枪
    CheckPlayerInTurret(line, "CROWS", "Seat Number");     // 检测大多数载具轮式载具，包括但不限于：坦克电摇位，斯崔克，30炮，LAV
    CheckPlayerInTurret(line, "Baseplate", "Seat Number"); // 检测迫击炮
    CheckPlayerInTurret(line, "Tripod", "Seat Number");    // 检测大多数固定武器
    CheckPlayerInTurret(line, "23Vehicle", "Seat Number");
    CheckPlayerInTurret(line, "Bowgun", "Seat Number"); // 检测VDV前排机枪位
#endif
}

// 检测玩家创建小队
void CheckPlayerCreatedSquad(char *line)
{
#if CHECKPLAYERCREATEDSQUAD
    // 换图
    if (CheckString(line, "created Squad") && CheckString(line, "HUANTU"))
    {
        char *PlayerName = GetGameLogString(line, "LogSquad: ", "%[^ ] (Online");
        char *Layer = GetGameLogString(line, "(Squad Name: ", "HUANTU %[^)]) on United");

        for (int i = 0; i < ChangeLayerPlayerNum; i++)
        {
            CheckPlaynameAndTime[i].playerName = PlayerName;
            // 再次申请
            if (CheckPlaynameAndTime[i].playerName == PlayerName)
            {
                if (LocalcombineDateTime_sec(localTime) - CheckPlaynameAndTime[i].checktime > 1800)
                {
                    printf("[%d.%d.%d-%d:%d:%d]", localTime->tm_year + 1900, localTime->tm_mon + 1, localTime->tm_mday, localTime->tm_hour, localTime->tm_min, localTime->tm_sec);
                    printf("ChangeLayer %s by %s\n", Layer, PlayerName);

                    CheckPlaynameAndTime[i].checktime = LocalcombineDateTime_sec(localTime);

                    char adminCommand[512];
                    snprintf(adminCommand, sizeof(adminCommand), "AdminBroadcast %s申请更换地图为%s，即将换图！！！", PlayerName, Layer);
                    RCONCommand(adminCommand, commandOutput);
                    //延时5秒
                    Delay_ms(5000);
                    snprintf(adminCommand, sizeof(adminCommand), "AdminChangeLayer %s", Layer);
                    RCONCommand(adminCommand, commandOutput);
                    if(CheckString(commandOutput,"layer Not Found"))
                    {
                        snprintf(adminCommand, sizeof(adminCommand), "AdminBroadcast %s申请换图，但未找到当前地图，请规范地图名称重新申请", PlayerName);
                        RCONCommand(adminCommand, commandOutput);
                        CheckPlaynameAndTime[i].checktime = LocalcombineDateTime_sec(localTime)-1800;
                    }
                }
                else
                {
                    unsigned int TimeDifference;
                    TimeDifference = LocalcombineDateTime_sec(localTime) - CheckPlaynameAndTime[i].checktime;
                    char adminCommand[512];
                    snprintf(adminCommand, sizeof(adminCommand), "AdminBroadcast %s 30分钟内已经执行过换图指令，请勿重复操作，距离下次申请还剩%ds", PlayerName, 1800 - TimeDifference);
                    RCONCommand(adminCommand, commandOutput);
                }
                break;
            }
            // 首次申请
            else if (CheckPlaynameAndTime[i].playerName != PlayerName && CheckPlaynameAndTime[i].playerName == NULL)
            {
                CheckPlaynameAndTime[i].playerName = PlayerName;
                CheckPlaynameAndTime[i].checktime = LocalcombineDateTime_sec(localTime);
                ChangeLayerPlayerNum++;

                printf("[%d.%d.%d-%d:%d:%d]", localTime->tm_year + 1900, localTime->tm_mon + 1, localTime->tm_mday, localTime->tm_hour, localTime->tm_min, localTime->tm_sec);
                printf("ChangeLayer %s by %s\n", Layer, PlayerName);

                char adminCommand[512];
                snprintf(adminCommand, sizeof(adminCommand), "AdminBroadcast %s申请更换地图为%s，即将换图！！！", PlayerName, Layer);
                RCONCommand(adminCommand, commandOutput);
                // 延时5秒
                Delay_ms(5000);
                snprintf(adminCommand, sizeof(adminCommand), "AdminChangeLayer %s", Layer);
                RCONCommand(adminCommand, commandOutput);
                if(CheckString(commandOutput,"layer Not Found"))
                {
                    snprintf(adminCommand, sizeof(adminCommand), "AdminBroadcast %s申请换图，但未找到当前地图，请规范地图名称重新申请", PlayerName);
                    RCONCommand(adminCommand, commandOutput);
                    CheckPlaynameAndTime[i].checktime = LocalcombineDateTime_sec(localTime)-1800;
                }
                break;
            }
        }
        for (int i = 0; i < ChangeLayerPlayerNum; i++)
        {
            // 超过30分钟的被清空
            if ((LocalcombineDateTime_sec(localTime) - CheckPlaynameAndTime[i].checktime > 1800) && (CheckPlaynameAndTime[i].checktime != 0))
            {
                CheckPlaynameAndTime[i].playerName = NULL;
                CheckPlaynameAndTime[i].checktime = 0;
                for (int j = i; j < ChangeLayerPlayerNum; j++)
                {
                    CheckPlaynameAndTime[j] = CheckPlaynameAndTime[j + 1];
                }
                ChangeLayerPlayerNum--;
            }
        }
        // 释放内存
        free(PlayerName);
        free(Layer);
    }
#endif
}
