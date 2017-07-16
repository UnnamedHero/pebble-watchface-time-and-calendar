#pragma once
#include "../../settings.h"

//typedef struct *tm

bool can_vibrate();
void update_timer();
void get_currect_time(DT_FORMAT, char *buffer);
int period_to_mins(PERIOD);
//void get_raw_time_ptr(struct tm *);
