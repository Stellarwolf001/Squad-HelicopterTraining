RCON.C程序为冲锋号四直升机训练服开源程序，感谢冲锋号四的开源精神

此脚本现安装于Squad自定义服务器红警三服_直升机训练专用服

此文件读取游戏日志通过RCON与服务器通信实现跳过开局时间、解除限制、检测玩家造成伤害、检测玩家进入炮手位等功能

本脚本完全使用C语言开发
使用环境：Linux，gcc

文件介绍：

main.c主函数

config.h功能选择

config.c文件配置

file.c检测日志更新状态

check.c检测游戏开始结束，玩家伤害等

command.c程序调用的指令

time.c获取当前时间进行打印

rcon.c通过rcon与服务器进行通信

使用方法：
将项目文件拷贝到Linux相应文件夹，修改config.c文件中的rconPath为rcon所在位置，serverPassword为服务器RCON密码，serverPort为服务器RCON端口，log_address为服务器日志文件位置

使用gcc编译器进行编译：

gcc rcon.c -o rcon

gcc main.c config.c file.c check.c command.c time.c -o main -std=gnu99

运行./main启用脚本
