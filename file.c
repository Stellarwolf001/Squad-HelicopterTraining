#include "./file.h"

char line[1024];
char previousline[1024];

long fileSize;
long prevfileSize;

#if LINUXORWINDOWS

long getFileSize(const char *filePath) 
{
    FILE *file = fopen(filePath, "r");

    if (file == NULL) {
        perror("Error opening log file");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0L, SEEK_END);
    long size = ftell(file);
    fclose(file);

    return size;
}

void monitorLogFile(const char *filePath) 
{
    FILE *file;
    while (1) 
    {
        // 打开日志文件
        file = fopen(filePath, "r");
        if (file == NULL) {
            perror("Error opening file");
            exit(EXIT_FAILURE);
        }

        // 定位到文件末尾并获取文件大小
        fseek(file, 0L, SEEK_END);
        fileSize = ftell(file);

        // 如果之前保存的文件大小不等于当前文件大小，表示文件已更新
        if (fileSize != prevfileSize)
        {        
            // 保存当前文件指针的位置
            long filePos = ftell(file);

            // 定位到文件起始
            fseek(file, prevfileSize, SEEK_SET);

            // 监测新的日志行
            while (fgets(line, sizeof(line), file) != NULL) 
            {
                //检测游戏开局及结束
                #if CHECKSTARTGAME
                    CheckStarkGame(line);
                    CheckOverGame(line);
                #endif
                //检测玩家加入
                #if CHECKPLAYERJOIN
                    CheckPlayerJoin(line);
                #endif
                //检测伤害
                #if (CHECKDAMAGE || CHECKDAMAGEWARNING)
                    CheckDamage(line);
                #endif
                //检测玩家进入炮手位
                #if CHECKPLAYERINTURRET
                    CheckPlayerInAllTurret(line);
                #endif
                //检测玩家创建小队
                #if CHECKPLAYERCREATEDSQUAD
                    CheckPlayerCreatedSquad(line);
                #endif
            }

            fseek(file, filePos, SEEK_SET);

            // 保存当前文件大小以备下次检查
            prevfileSize = fileSize;
            Delay_ms(10);
        }

        // 关闭文件
        fclose(file);
    }
}

#else
long getFileSize(const char *filePath) 
{
    FILE *file = fopen(filePath, "r");

    if (file == NULL) {
        perror("Error opening log file");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fclose(file);

    return size;
}

void monitorLogFile(const char *filePath) 
{
    int fd = inotify_init();
    
    if (fd == -1) 
    {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }

    int wd = inotify_add_watch(fd, filePath, IN_MODIFY);

    if (wd == -1) 
    {
        perror("inotify_add_watch");
        exit(EXIT_FAILURE);
    }

    char buffer[BUF_LEN];
    long offset = fileSize;  //初始文件偏移量设为 fileSize

    while (1)
    {
        ssize_t bytesRead = read(fd, buffer, BUF_LEN);

        if (bytesRead <= 0) 
        {
            perror("read");
            exit(EXIT_FAILURE);
        }

        for (char *ptr = buffer; ptr < buffer + bytesRead;) 
        {
            struct inotify_event *event = (struct inotify_event *)ptr;

            if (event->mask & IN_MODIFY) 
            {
                //读取更新的内容
                readUpdatedContent(filePath, &offset);
            }

            ptr += EVENT_SIZE + event->len;
        }
    }

    inotify_rm_watch(fd, wd);
    close(fd);
}

void readUpdatedContent(const char *filePath, long *offset) 
{
    FILE *file = fopen(filePath, "r");

    if (file == NULL) 
    {
        perror("Error opening log file");
        exit(EXIT_FAILURE);
    }

    // 定位到上次的偏移量
    fseek(file, *offset, SEEK_SET);

    while (fgets(line, sizeof(line), file) != NULL) 
    {
        // 确保 line 的长度不超过缓冲区大小
        if (strlen(line) >= sizeof(line) - 1)
        {
            fprintf(stderr, "Error: Line exceeds buffer size\n");
            exit(EXIT_FAILURE);
        }

        //检测游戏开局及结束
        #if CHECKSTARTGAME
            CheckStarkGame(line);
            CheckOverGame(line);
        #endif
        //检测玩家加入
        #if CHECKPLAYERJOIN
            CheckPlayerJoin(line);
        #endif
        //检测伤害
        #if (CHECKDAMAGE || CHECKDAMAGEWARNING)
            CheckDamage(line);
        #endif
        //检测玩家进入炮手位
        #if CHECKPLAYERINTURRET
            CheckPlayerInAllTurret(line);
        #endif
        //检测玩家创建小队
        #if CHECKPLAYERCREATEDSQUAD
            CheckPlayerCreatedSquad(line);
        #endif
        // 清空 line 缓冲区
        memset(line, 0, sizeof(line));
    }

    // 更新偏移量
    *offset = ftell(file);

    // 关闭文件
    fclose(file);
}

#endif
