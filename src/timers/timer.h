#ifndef TIMER_H
#define TIMER_H

#include "Arduino.h"
#include "package/package.h"

extern const uint8_t GATEWAY_ID;

int setupTimers(void);
void IRAM_ATTR onSendTimer(void);
// void IRAM_ATTR onResponseTimer(void);

#endif