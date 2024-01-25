/*此代码主要功能为：
 *提取更新后的日志
 *判断日志中是否存在关键字来检测开局，结束，击杀，进入炮手位等信息
 *通过RCON发送指令与服务器通信实现功能自动化
 *本人刚刚入门，代码风格较为浅显，大佬请见谅
 *
 *请将config.c文件下内容内容进行修改
 * *rconPath = "YourRconAddress";
 * *serverPassword = "-PYourRCONPassword";
 * *serverPort = "-pYourRCONPort";
 * *log_address = "YourSquadGameAddress";
 * 
 * gcc main.c config.c file.c check.c command.c time.c -o main -std=gnu99
 * Squad红警_Stellarwolf编写
*/

#include "./config.h"
#include "./file.h"
#include "./check.h"
#include "./command.h"
#include "./time.h"

int main() 
{
    system("clear");
    time_updata();
    printf("[%d.%d.%d-%d:%d:%d]",localTime->tm_year + 1900,localTime->tm_mon + 1,localTime->tm_mday,localTime->tm_hour,localTime->tm_min,localTime->tm_sec);
    printf("脚本启动成功!\n");

    // 获取文件的当前大小
    fileSize = getFileSize(log_address);
    monitorLogFile(log_address);
}
