#pragma once

#include "../../settings.h"


bool is_quiet_time();
//void update_timer();
void get_currect_time(DT_FORMAT, char *buffer);
void get_current_date(char *format, char *out);
bool is_time_to(uint32_t , PERIOD );
//int period_to_mins(PERIOD);
// uint8_t get_hour_from_str(char*);
// uint8_t get_mins_from_str(char*);
