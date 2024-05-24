/*此代码主要功能为：
 *提取更新后的日志
 *判断日志中是否存在关键字来检测开局，结束，击杀，进入炮手位等信息
 *通过RCON发送指令与服务器通信实现自定义直升机训练服的自动管理功能
 *本人刚刚入门，代码风格较为浅显，大佬请见谅
 *
 *请将config.c文件下内容内容进行修改
 * 
 * Linux    gcc main.c config.c file.c check.c command.c time.c rcon.c -o SquadHelicopterTraining -std=gnu99
 * Windows  gcc main.c config.c file.c check.c command.c time.c rcon.c -o SquadHelicopterTraining -std=gnu99 -lws2_32
 * 
 * Squad红警_Stellarwolf编写
*/

#include "./config.h"

int main() 
{
    #if LINUXORWINDOWS
    system("chcp 65001");
    system("cls");
    #else
    system("clear");
    #endif
    time_updata();
    printf("[%d.%d.%d-%d:%d:%d]",localTime->tm_year + 1900,localTime->tm_mon + 1,localTime->tm_mday,localTime->tm_hour,localTime->tm_min,localTime->tm_sec);
    printf("脚本启动成功!\n");

    //获取文件的当前大小
    prevfileSize = getFileSize(log_address);
    monitorLogFile(log_address);
}
