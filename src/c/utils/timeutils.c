#include <pebble.h>
#include "include/timeutils.h"

struct tm *tick_time;

bool can_vibrate() {
  bool res = settings_get_RespectQuietTime() ? !quiet_time_is_active() : true;
  // APP_LOG(APP_LOG_LEVEL_DEBUG, "quet time?: %s", quiet_time_is_active() ? "true" : "false");
  // APP_LOG(APP_LOG_LEVEL_DEBUG, "can bzzzz: %s", res ? "true" : "false");
  BatteryChargeState cs = battery_state_service_peek();
  return res && !(cs.is_charging || cs.is_plugged);

}



void update_timer() {
  time_t temp = time(NULL);
  tick_time = localtime(&temp);
}

// void get_raw_time_ptr(struct tm *tm_ptr) {
//   update_timer();
//   tm_ptr = tick_time;
// }

char* get_currect_time(DT_FORMAT dtf) {
  static char d_buffer[32];
  char format[12];
  update_timer();

  switch (dtf) {
    case YYYY_MM_DD:
      strcpy(format, "%Y.%m.%d");
      break;
    case CLOCK_FORMAT:
      strcpy(format, settings_get_clockformat());
      break;
    }
  strftime(d_buffer, sizeof(d_buffer), format, tick_time);
  return d_buffer;
}


static int isleap(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int get_days_in_month(int month, int year) {
    static const int days[2][12] = {
        {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
        {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
    };
    int leap = isleap(year);
    return days[leap][month - 1];
}
