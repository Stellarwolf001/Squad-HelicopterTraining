#ifndef CHECK__H
#define CHECK__H

#include "./command.h"

void CheckStarkGame(char *line);
void CheckOverGame(char *line);
void CheckPlayerJoin(char *line);
void CheckDamage(char *line);
void CheckPlayerInTurret(char *line, char *vehicleType, char *seatNumber);
void CheckPlayerInAllTurret(char *line);
int CheckNonHelicopter(const char *line);
#endif