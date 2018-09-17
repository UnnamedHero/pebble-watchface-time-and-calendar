#pragma once

#include "../../settings.h"

void init_time_utils();
tm* get_Time();
int get_mins(int hours, int mins);

bool is_quiet_time();
void get_currect_time(DT_FORMAT, char *buffer);
void get_current_date(char *format, char *out);
bool is_time_to(uint32_t , PERIOD);
bool is_within_range(int now, int begin, int end);
int get_min(int hours, int mins);
