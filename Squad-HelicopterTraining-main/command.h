#ifndef COMMAND__H
#define COMMAND__H

#include "./config.h"
#include "./time.h"

#define CHANGELAYER     1
#define PLAYERJOIN      2

#define FULLCOMMAND_SIZE    512
#define COMMANDOUTPUT_SIZE  1024

extern const char *rconPath;
extern const char *serverPassword;
extern const char *serverPort;
extern const char *log_address;

extern const char *commands[];
extern size_t numCommands;
extern char fullCommand[FULLCOMMAND_SIZE];
extern char commandOutput[COMMANDOUTPUT_SIZE];
extern int commandExecuted;

int executeCommand(const char *command, char *output);
int RCONCommand(const char *command, char *output);
int CheckstringAndTime(char *line, char *string);
int GetGamePlayerNum();
char* GetGamePlayerName(const char *line, const char *frontString, const char *searchString);
void CheckPlayerInTurret(char *line, char *vehicleType, char *seatNumber);
void Skip_Time(struct tm *localTime,int command);

#endif