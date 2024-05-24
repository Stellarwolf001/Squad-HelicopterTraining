#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

/*
*宏定义适配系统
* 0 适配系统为Linux
* 1 适配系统为Windows
*/
#define LINUXORWINDOWS              0

//需要实现功能选择
#define CHECKSTARTGAME              1   //检测是否开局
#define CHECKPLAYERJOIN             1   //检测是否有玩家加入游戏：服务器无人后会保持暂停状态，关闭后如若服务器暂停无法实现脚本功能
#define CHECKDAMAGE                 1   //检测是否有玩家造成恶意伤害
#define CHECKPLAYERINTURRET         1   //检测是否有玩家进入炮手位
#define CHECKDAMAGEWARNING          1   //检测玩家受到伤害提示击杀者
#define CHECKPLAYERCREATEDSQUAD     1   //检测玩家创建小队(检测是否换图)

#if LINUXORWINDOWS
#include <winsock2.h>
#include <Windows.h>
#else
#include <sys/inotify.h>
#include <sys/wait.h>
#endif

#include "./file.h"
#include "./check.h"
#include "./command.h"
#include "./mytime.h"
#include "./rcon.h"

extern const char   address[64];
extern const short  rconport;
extern const char   password[512];
extern const char   *log_address;
#endif