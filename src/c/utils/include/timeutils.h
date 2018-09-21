#pragma once

#include "../../settings.h"
#include "timelib.h"

void init_time_utils();
struct tm* get_Time();


bool is_quiet_time();
void get_currect_time(DT_FORMAT, char *buffer);
void get_current_date(char *format, char *out);
bool is_time_to(uint32_t , PERIOD);

int get_min(int hours, int mins);
