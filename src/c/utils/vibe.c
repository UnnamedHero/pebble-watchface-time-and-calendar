#include <pebble.h>
#include "include/vibe.h"
#include "include/timeutils.h"
#include "include/ticktimerhelper.h"
#include "../settings.h"

static void prv_ticktimer(struct tm*);
static uint32_t segments[] = {100, 200, 100, 200, 100};
static VibePattern pat = {
  .durations = segments,
  .num_segments = ARRAY_LENGTH(segments)
};

void init_vibrate() {
  ticktimerhelper_register(prv_ticktimer);
}

static void prv_ticktimer(struct tm* timer) {

  if (!(settings_get_VibratePeriodic() || can_vibrate())) {
    return;
  }  
  bool do_vibr = false;

  switch (settings_get_VibratePeriodicPeroid()) {
    case P_15MIN:
      do_vibr = timer->tm_min % 15 == 0;
      break;
    case P_30MIN:
      do_vibr = timer->tm_min % 30 == 0;
      break;
    case P_1H:
      do_vibr = timer->tm_min == 0;
      break;
    case P_3H:
      do_vibr = timer->tm_hour % 3 == 0;
      break;
    default:
      break;
  }

  if (do_vibr) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "BzZzzZzzZzzZzzzzz");
    do_vibrate(settings_get_VibratePeriodicType());
  }
}

void do_vibrate(VIBE vibe_pattern) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "VIBE : %i", vibe_pattern);
  switch (vibe_pattern) {
    case VP_SHORT:
      vibes_short_pulse();
      break;
    case VP_DOUBLE_SHORT:
      vibes_double_pulse();
      break;
    case VP_TRIPLE_SHORT:
      vibes_enqueue_custom_pattern(pat);
      break;
    case VP_LONG:
      vibes_long_pulse();
      break;
  }
}

bool can_vibrate() {
  bool qt = is_quiet_time();
  bool res = settings_get_RespectQuietTime() ? qt : true;
  BatteryChargeState cs = battery_state_service_peek();
  return res && !(cs.is_charging || cs.is_plugged);

}