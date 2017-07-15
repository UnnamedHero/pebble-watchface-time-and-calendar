#include <pebble.h>
#include "twc-ng.h"
#include "settings.h"
#include "windows/time-window.h"
#include "utils/include/timeutils.h"
#include "utils/include/vibe.h"

static bool s_js_ready;

static void prv_periodic_vibrate(struct tm *timer) {
  if (timer->tm_sec % 10 == 1) {
  if (settings_get_VibratePeriodic() && can_vibrate()) {
    do_vibrate(settings_get_VibratePeriodicType());
  }
}
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  //update_time();
  prv_periodic_vibrate(tick_time);

  window_update_time(tick_time, units_changed);
//  change_pic(tick_time->tm_min);
//  if (tick_time->tm_min % 15 == 0) {
    //update_weather();
//  }
}




void settings_update_handler(UPDATE_FLAG f) {
  if (f == UF_WEATHER) {
//    init_weather(settings_get_weather_apikey());
  }
}

static void prv_inbox_received_handler(DictionaryIterator *iter, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Inbox message received");
  Tuple *js_ready_t = dict_find(iter, MESSAGE_KEY_JSReady);
  if (js_ready_t) {
    s_js_ready = true;
    ready_for_weather();
    //init_weather(settings_get_weather_apikey());
  }

  Tuple *config_marker = dict_find(iter, MESSAGE_KEY_ConfigMarker);
  if (config_marker) {
    populate_settings(iter, context);
  }

  Tuple *weather_marker = dict_find(iter, MESSAGE_KEY_WeatherMarker);
  if (weather_marker) {
    Tuple *weather_forecast = dict_find(iter, MESSAGE_KEY_WeatherMarkerForecast);
    if (weather_forecast) {

    } else {
      simple_weather_update(iter, context);
    }
  }

}
static void accel_tap_handler(AccelAxisType axis, int32_t direction) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "SHAKE IT BABY!!!");
}

static void prv_init() {
  init_settings(settings_update_handler);

  app_message_register_inbox_received(prv_inbox_received_handler);
  app_message_open(256, 128);
  init_time_window();

  const bool animated = true;
  window_stack_push(get_time_window(), animated);

//  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  tick_timer_service_subscribe(SECOND_UNIT|MINUTE_UNIT|HOUR_UNIT|DAY_UNIT, tick_handler);
//  update_time();
  accel_tap_service_subscribe(accel_tap_handler);
}

static void prv_deinit(void) {
 deinit_time_window();
}

int main(void) {
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "Locale: %s", i18n_get_system_locale());
  prv_init();
  app_event_loop();
  prv_deinit();
}
