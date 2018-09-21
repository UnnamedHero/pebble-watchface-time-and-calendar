#include "include/timelib.h"

bool is_within_range(int current, int begin, int end) {
  if (current == begin || current == end) {
    return true;
  }
  bool is_ragged_interval = begin > end;
  int max = begin >= end ? begin : end;
  int min = begin >= end ? end : begin;  
  bool is_between = current >= min && current <= max;
  if (!is_ragged_interval) {
    return is_between;
  }
  return !is_between;
}

int get_mins(int hour, int min) {
  return (hour * MINUTES_PER_HOUR) + min;
}