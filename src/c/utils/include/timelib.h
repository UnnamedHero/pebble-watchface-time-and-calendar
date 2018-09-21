#pragma once
#include <stdbool.h>
#define MINUTES_PER_HOUR 60

bool is_within_range(int now, int begin, int end);
int get_mins(int hours, int mins);
