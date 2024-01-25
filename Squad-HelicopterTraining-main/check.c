#include "./check.h"

void CheckStarkGame(char *line)
{            
    if(CheckstringAndTime(line,"StartLoadingDestination"))
    {
        Skip_Time(localTime);
    }
}

void CheckOverGame(char *line)
{            
    if(CheckstringAndTime(line,"DetermineMatchWinner"))
    {
        RCONCommand("AdminSlomo 20", commandOutput);
    }
}

void CheckPlayerJoin(char *line)
{            
    if(CheckstringAndTime(line,"Join succeeded"))
    {   
        char *PlayerName = GetGamePlayerName(line, "LogNet: ", "Join succeeded: %[^ ]");

        printf("[%d.%d.%d-%d:%d:%d]",localTime->tm_year + 1900,localTime->tm_mon + 1,localTime->tm_mday,localTime->tm_hour,localTime->tm_min,localTime->tm_sec);
        printf("PlayerJoin: %s", PlayerName); 

        if(GetGamePlayerNum() == 1)
        {
            Skip_Time(localTime);
        }
        
        //释放内存
        free(PlayerName);
    }
}

//检测是否有玩家造成恶意伤害
void CheckKill(char *line)
{
    if(CheckstringAndTime(line,"Player Controller") && CheckNonHelicopter(line))
    {
        char *KillerPlayerName = GetGamePlayerName(line, "from ", "%[^ ] (Online");
        char *VictimPlayerName = GetGamePlayerName(line, "LogSquad: Player:", "%[^ ] ActualDamage");
            
        if (KillerPlayerName != NULL && VictimPlayerName != NULL && strcmp(KillerPlayerName, VictimPlayerName) != 0 && strcmp(VictimPlayerName, "nullptr") != 0)
        {
            printf("[%d.%d.%d-%d:%d:%d]",localTime->tm_year + 1900,localTime->tm_mon + 1,localTime->tm_mday,localTime->tm_hour,localTime->tm_min,localTime->tm_sec);
            printf("Was kicked for a kill:Player %s\n", KillerPlayerName);

            char adminCommand[512];
            snprintf(adminCommand, sizeof(adminCommand), "AdminKick %s 直升机训练场请勿对玩家造成伤害", KillerPlayerName);
            RCONCommand(adminCommand, commandOutput);
            snprintf(adminCommand, sizeof(adminCommand), "AdminBroadcast %s 对%s造成伤害,已被自动踢出", KillerPlayerName,VictimPlayerName);
            RCONCommand(adminCommand, commandOutput);

            //释放内存
            free(KillerPlayerName);
            free(VictimPlayerName);
        }
    }
}

//检测是否有玩家进入炮手位
void CheckPlayerInTurret(char *line, char *vehicleType, char *seatNumber)
{
    if (CheckstringAndTime(line, vehicleType) && CheckstringAndTime(line, seatNumber) && CheckstringAndTime(line, "OnPossess()") && CheckNonHelicopter(line))
    {
        char *PlayerName = GetGamePlayerName(line, "[DedicatedServer]", "OnPossess(): PC=%[^ ] (Online");
            
        printf("[%d.%d.%d-%d:%d:%d]",localTime->tm_year + 1900,localTime->tm_mon + 1,localTime->tm_mday,localTime->tm_hour,localTime->tm_min,localTime->tm_sec);
        printf("Was kicked for entering the Turret:Player %s\n", PlayerName);

        char adminCommand[512];
        snprintf(adminCommand, sizeof(adminCommand), "AdminKick %s 直升机训练场请勿进入载具炮手位！！！", PlayerName);
        RCONCommand(adminCommand, commandOutput);
        snprintf(adminCommand, sizeof(adminCommand), "AdminBroadcast %s 已被自动踢出，直升机训练场请勿进入载具炮手位！！！", PlayerName);
        RCONCommand(adminCommand, commandOutput);

        //释放内存
        free(PlayerName);
    }
}

//检测字符串不包含直升机
int CheckNonHelicopter(const char *line)
{
    if( !strstr(line, "UH60")   && !strstr(line, "MI8")     && !strstr(line, "UH1Y") && \
        !strstr(line, "Z8G")    && !strstr(line, "MRH90")   && !strstr(line, "MI17") && \
        !strstr(line, "CH146")  && !strstr(line, "SA330")   && !strstr(line, "Z8J")  && \
        !strstr(line, "UH1H"))
        return 1;
    else
        return 0;
}

void CheckPlayerInAllTurret(char *line)
{
#if CHECKPLAYERINTURRET
    CheckPlayerInTurret(line, "Turret",     "Seat Number");     //检测大多数载具，包括但不限于：侦察车，坦克炮手，飞机机枪
    CheckPlayerInTurret(line, "turret",     "Seat Number");     //检测大多数载具，包括但不限于：侦察车，坦克炮手，飞机机枪
    CheckPlayerInTurret(line, "CROWS",      "Seat Number");     //检测大多数载具轮式载具，包括但不限于：坦克电摇位，斯崔克，30炮，LAV
    CheckPlayerInTurret(line, "Baseplate",  "Seat Number");     //检测迫击炮
    CheckPlayerInTurret(line, "Tripod",     "Seat Number");     //检测大多数固定武器
    CheckPlayerInTurret(line, "23Vehicle",  "Seat Number");
    CheckPlayerInTurret(line, "Bowgun",     "Seat Number");     //检测VDV前排机枪位
#endif
}