#ifndef FILE__H
#define FILE__H

#include "./config.h"
#include "./check.h"
#include "./command.h"
#include "./time.h"

#define EVENT_SIZE  (sizeof(struct inotify_event))
#define BUF_LEN     (1024 * (EVENT_SIZE + 16))

extern char line[512];
extern long fileSize;

long getFileSize(const char *filePath);
void monitorLogFile(const char *filePath);
void readUpdatedContent(const char *filePath, long *offset);

#endif