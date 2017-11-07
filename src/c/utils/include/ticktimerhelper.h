#pragma once

#if defined (TESTS)
typedef void(*ticktimerhandler)(int);
void ticktimerhelper_emulate(int);
#else
#include <pebble.h>
typedef void(*ticktimerhandler)(struct tm *);
#endif


void start_ticktimerhelper();
void stop_ticktimerhelper();
void reload_timer();
void ticktimerhelper_register(ticktimerhandler);
void ticktimerhelper_register_clock(ticktimerhandler);

