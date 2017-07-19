#include <pebble.h>
#include "include/timeutils.h"
#include "../settings.h"

#if defined (PBL_PLATFORM_APLITE)
static int prv_get_mins(int hour, int min) {
  return hour * MINUTES_PER_HOUR + min;
}

static bool prv_is_between_numbers(int begin, int end, int now) {
  bool within_24h = begin < end;
  if (within_24h) {    
    return (now > begin) && (now < end);
  } 
  return (((now < begin) && (now < end)) || ((now > begin) && (now > end)));
}

bool aplite_quiet_time_is_active() {
  struct tm *tick_time;
  time_t temp = time(NULL);
  tick_time = localtime(&temp);
  
  int current_min_of_a_day = prv_get_mins(tick_time->tm_hour, tick_time->tm_min);  
  int qtmins_begin = prv_get_mins(settings_get_QTHourBegin(), settings_get_QTMinBegin());
  int qtmins_end = prv_get_mins(settings_get_QTHourEnd(), settings_get_QTMinEnd());  
  return prv_is_between_numbers(qtmins_begin, qtmins_end, current_min_of_a_day);
}
#endif

bool can_vibrate() {
#if defined (PBL_PLATFORM_APLITE)
  bool qt = !aplite_quiet_time_is_active();
#else
  bool qt = !quiet_time_is_active();
#endif
  bool res = settings_get_RespectQuietTime() ? qt : true;
  BatteryChargeState cs = battery_state_service_peek();
  return res && !(cs.is_charging || cs.is_plugged);
}


void get_currect_time(DT_FORMAT dtf, char *buffer) {

  struct tm *tick_time;
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
