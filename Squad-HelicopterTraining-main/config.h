#ifndef CONFIG_H
#define CONFIG_H

#include <sys/inotify.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

//需要实现功能选择
#define CHECKSTARTGAME              1   //检测是否开局
#define CHECKPLAYERJOIN             1   //检测是否有玩家加入游戏：服务器无人后会保持暂停状态，关闭后如若服务器暂停无法实现脚本功能
#define CHECKKILL                   1   //检测是否有玩家造成恶意伤害
#define CHECKPLAYERINTURRET         1   //检测是否有玩家进入炮手位

extern const char *rconPath;
extern const char *serverPassword;
extern const char *serverPort;
extern const char *log_address;

#endif