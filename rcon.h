#ifndef __RCON_H
#define __RCON_H

#include "config.h"

#if LINUXORWINDOWS
#define MY_CONST       (const char*)
#define MY_CHAR        (char*)
#else
#define MY_CONST
#define MY_CHAR
#endif

int RconSendCommand(const char *command,char *output);

#endif
