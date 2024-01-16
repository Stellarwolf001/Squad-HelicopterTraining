RCON.C程序为冲锋号四直升机训练服改编的Windows版本
感谢冲锋号四的开源精神

此脚本现安装于Squad自定义服务器红警三服_直升机训练专用服
此文件仅实现跳过开局时间及解除限制功能

使用环境：Windows，gcc

使用方法：
打开SkipTime.c文件，将rconPath，serverPassword，serverPort，log_address分别改为rcon.exe文件的地址，服务器RCON的密码，服务器RCON的端口，服务器LOG文件地址

使用gcc编译器进行编译：

gcc -o rcon rcon.c -lws2_32

gcc -o SkipTime SkipTime.c

将生成的SkipTime.exe及rcon.exe拷贝到服务器中
运行SkipTime.exe启动程序
