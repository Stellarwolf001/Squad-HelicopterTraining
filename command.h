#ifndef COMMAND__H
#define COMMAND__H

#include "./config.h"

#define CHANGELAYER     1
#define PLAYERJOIN      2

#define FULLCOMMAND_SIZE    512
#define COMMANDOUTPUT_SIZE  1024

extern const char *log_address;

extern const char *commands[];
extern size_t numCommands;
extern char fullCommand[FULLCOMMAND_SIZE];
extern char commandOutput[COMMANDOUTPUT_SIZE];
extern int commandExecuted;

int RCONCommand(const char *command, char *output);
int CheckString(char *line, char *string);
int GetGamePlayerNum();
char* GetGameLogString(const char *line, const char *frontString, const char *searchString);
void CheckPlayerInTurret(char *line, char *vehicleType, char *seatNumber);
void Skip_Time(struct tm *localTime,int command);

#endif