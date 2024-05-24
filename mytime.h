#ifndef TIME__H
#define TIME__H

#include "./config.h"

extern time_t currentTime;
extern struct tm *localTime;

//更新时间
void time_updata(void);
unsigned int LocalcombineDateTime_sec(struct tm *localTime);
void Delay_ms(int ms);
#endif