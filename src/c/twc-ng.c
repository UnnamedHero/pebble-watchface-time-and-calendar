#include <pebble.h>
#include "twc-ng.h"
#include "settings.h"
#include "windows/time-window.h"
#include "utils/include/timeutils.h"
#include "utils/include/vibe.h"
#include "3rdparty/locale_framework/localize.h"

static bool s_js_ready;

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


static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  //update_time();
  if (units_changed != SECOND_UNIT) {
    prv_periodic_vibrate(tick_time);
  }
  window_update_time(tick_time);
}

void settings_update_handler(UPDATE_FLAG f) {
//  if (f == UF_WEATHER) {
    ready_for_weather();
//  }
  time_window_force_redraw();
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
  app_message_open(400, 128);
  init_time_window();

  const bool animated = true;
  window_stack_push(get_time_window(), animated);

  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
//  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
//  update_time();
  accel_tap_service_subscribe(accel_tap_handler);
}

static void prv_deinit(void) {
 deinit_time_window();
}

int main(void) {
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "Locale: %s", i18n_get_system_locale());
  locale_init();
  prv_init();
  app_event_loop();
  prv_deinit();
}
