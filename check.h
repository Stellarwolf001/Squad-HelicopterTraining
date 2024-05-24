#ifndef CHECK__H
#define CHECK__H

#include "./command.h"

typedef struct 
{
    char *playerName;
    unsigned int checktime;
}CheckChangeLayerTime;

void CheckStarkGame(char *line);
void CheckOverGame(char *line);
void CheckPlayerJoin(char *line);
void CheckDamage(char *line);
void CheckPlayerInTurret(char *line, char *vehicleType, char *seatNumber);
void CheckPlayerCreatedSquad(char *line);
void CheckPlayerInAllTurret(char *line);
int CheckNonHelicopter(const char *line);
#endif