#include <pebble.h>
#include "twc-ng.h"
#include "settings.h"
#include "windows/include/time-window.h"
#include "windows/include/forecast-window.h"
#include "utils/include/timeutils.h"
#include "utils/include/vibe.h"
#include "utils/include/messagesystem.h"
#include "3rdparty/locale_framework/localize.h"


static bool main_window_active = true;
static AppTimer *s_timeout_timer;
static const int timeout = 20000;
static void prv_timer_timeout_handler(void*);

static void prv_periodic_vibrate(struct tm *timer) {
  if (!(settings_get_VibratePeriodic() && can_vibrate())) {
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
    do_vibrate(settings_get_VibratePeriodicType());
  }
}

static void toggle_windows() {
  bool now = main_window_active;
  const bool animated = true;
  //window_stack_push(get_time_window(), animated);
  if (now) { 
    window_stack_push(get_forecast_window(), animated);  
    s_timeout_timer = app_timer_register(timeout, prv_timer_timeout_handler, NULL);
  } else {
    app_timer_cancel(s_timeout_timer);
    window_stack_push(get_time_window(), animated);
  }
  main_window_active = !now;
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  //update_time();
  if (units_changed != SECOND_UNIT) {
    prv_periodic_vibrate(tick_time);
  }
  window_update_time(tick_time);
}

void settings_update_handler(UPDATE_FLAG f) {
//  if (f == UF_WEATHER) {
  bool force_weather_update = true;
    ready_for_weather(force_weather_update);
//  }
  time_window_force_redraw();
}



static void accel_tap_handler(AccelAxisType axis, int32_t direction) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "SHAKE IT BABY!!!");
  toggle_windows();
}

static void prv_init() {
  init_message_system();
  init_settings(settings_update_handler);


  init_time_window();
  init_forecast_window();

  const bool animated = true;
  //window_stack_push(get_time_window(), animated);
  window_stack_push(get_time_window(), animated);

  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
//  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
//  update_time();
  accel_tap_service_subscribe(accel_tap_handler);
}

static void prv_deinit(void) {
 deinit_time_window();
 deinit_message_system();
}

static void prv_timer_timeout_handler (void *context) {
  toggle_windows();
}

int main(void) {
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "Locale: %s", i18n_get_system_locale());
  locale_init();
  prv_init();
  app_event_loop();
  prv_deinit();
}
