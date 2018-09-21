#pragma once
#include <stdbool.h>

#if !defined(MINUTES_PER_HOUR)
  #define MINUTES_PER_HOUR 60
#endif

bool is_within_range(int now, int begin, int end);
int get_mins(int hours, int mins);
