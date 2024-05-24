编译环境；gcc

#此代码主要功能为：
#提取日志信息
#判断日志中是否存在关键字来检测开局，结束，击杀，进入炮手位等信息
#通过RCON发送指令与服务器通信实现自定义直升机训练服的自动管理功能
#本人刚刚入门，代码风格较为浅显，大佬请见谅

使用请在config.h文件中修改参数选择适配系统
请将config.c文件中的address、rconport、password、log_address分别修改为你的服务器IP、rcon端口、rcon密码、log日志路径
 
Linux    编译
gcc main.c config.c file.c check.c command.c time.c rcon.c -o SquadHelicopterTraining -std=gnu99

Windows  编译
gcc main.c config.c file.c check.c command.c time.c rcon.c -o SquadHelicopterTraining -std=gnu99 -lws2_32

编译后运行SquadHelicopterTraining程序以执行脚本

#Squad红警_Stellarwolf编写
#如有疑问或见解请添加本人qq2221550341
