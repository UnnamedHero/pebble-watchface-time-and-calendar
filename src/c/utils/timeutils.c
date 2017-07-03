#include <pebble.h>
#include "include/timeutils.h"
#include "../settings.h"


bool can_vibrate() {
  bool res = settings_get_RespectQuietTime() ? !quiet_time_is_active() : true;
  // APP_LOG(APP_LOG_LEVEL_DEBUG, "quet time?: %s", quiet_time_is_active() ? "true" : "false");
  // APP_LOG(APP_LOG_LEVEL_DEBUG, "can bzzzz: %s", res ? "true" : "false");
  BatteryChargeState cs = battery_state_service_peek();
  return res && !(cs.is_charging || cs.is_plugged);

}
