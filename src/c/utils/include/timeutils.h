#pragma once
#include "../../settings.h"

//typedef struct *tm

bool can_vibrate();
void update_timer();
void get_currect_time(DT_FORMAT, char *buffer);
int period_to_mins(PERIOD);
uint8_t get_hour_from_str(char*);
uint8_t get_mins_from_str(char*);
//void get_raw_time_ptr(struct tm *);
