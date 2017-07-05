#include <pebble.h>
#include "include/timeutils.h"



bool can_vibrate() {
  bool res = settings_get_RespectQuietTime() ? !quiet_time_is_active() : true;
  // APP_LOG(APP_LOG_LEVEL_DEBUG, "quet time?: %s", quiet_time_is_active() ? "true" : "false");
  // APP_LOG(APP_LOG_LEVEL_DEBUG, "can bzzzz: %s", res ? "true" : "false");
  BatteryChargeState cs = battery_state_service_peek();
  return res && !(cs.is_charging || cs.is_plugged);

}


char* get_currect_time(DT_FORMAT dtf) {
  static char d_buffer[32];
  char format[12];

  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);



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
