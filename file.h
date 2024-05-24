#ifndef FILE__H
#define FILE__H

#include "./config.h"


extern char line[1024];
extern long fileSize;
extern long prevfileSize;

#if LINUXORWINDOWS
long getFileSize(const char *filePath);
void monitorLogFile(const char *filePath);
#else
#define EVENT_SIZE  (sizeof(struct inotify_event))
#define BUF_LEN     (1024 * (EVENT_SIZE + 16))
long getFileSize(const char *filePath);
void monitorLogFile(const char *filePath);
void readUpdatedContent(const char *filePath, long *offset);
#endif

#endif