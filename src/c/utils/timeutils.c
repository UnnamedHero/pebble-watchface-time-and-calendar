#include <pebble.h>
#include "include/timeutils.h"

static struct tm *tick_time;

#if defined (PBL_PLATFORM_APLITE)
bool aplite_quiet_time_is_active() {
  return true;
}
#endif

bool can_vibrate() {
#if defined (PBL_PLATFORM_APLITE)
  bool qt = !aplite_quiet_time_is_active();
#else
  bool qt = !quiet_time_is_active();
#endif
  bool res = settings_get_RespectQuietTime() ? qt : true;

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

void get_currect_time(DT_FORMAT dtf, char *buffer) {
  time_t temp = time(NULL);
  tick_time = localtime(&temp);
  char d_buffer[32];
  char format[12];
  //update_timer();
  //char fmt[] = "%Y.%m.%d";
  switch (dtf) {
    case YYYY_MM_DD:
      strcpy(format, "%Y.%m.%d");
      break;
    case CLOCK_FORMAT:
      strcpy(format, "%H:%M");
      break;
    }
  strftime(d_buffer, sizeof(d_buffer), format, tick_time);
  strcpy(buffer, d_buffer);
//  return d_buffer;
}

// int get_unixtime() {
//   update_timer();
//
// }

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

int period_to_mins(PERIOD per) {
  int mins = 0;
  switch (per) {
    case P_15MIN:
      mins = 15;
      break;
    case P_30MIN:
      mins = 30;
      break;
    case P_1H:
      mins = 60;
      break;
    case P_3H:
      mins = 180;
      break;
     case P_6H:
      mins = 360;
      break;
  }
  return mins;
}

uint8_t get_hour_from_str(char* str) {
  // int hour;
  // sscanf(str, "%d", &hour);
  // APP_LOG(APP_LOG_LEVEL_DEBUG, "STRING: %d", hour);
   return 0;
}

uint8_t get_mins_from_str(char* str) {
  return 0;
}
