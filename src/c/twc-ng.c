#include <pebble.h>
#include "twc-ng.h"
//#include "localize.h"
#include "settings.h"
//#include "weather.h"
//#include "layers/top-panel-layer.h"
#include "windows/time-window.h"
#include "utils/include/timeutils.h"
#include "utils/include/vibe.h"

static bool s_js_ready;

//---------bitmaps





//---------functions


// static void prv_select_click_handler(ClickRecognizerRef recognizer, void *context) {
//   text_layer_set_text(s_time_layer, "Select");
// }
//
// static void prv_up_click_handler(ClickRecognizerRef recognizer, void *context) {
//   text_layer_set_text(s_text_layer, "Up");
// }
//
// static void prv_down_click_handler(ClickRecognizerRef recognizer, void *context) {
//   text_layer_set_text(s_text_layer, "Down");
// }
//
// static void prv_click_config_provider(void *context) {
//   window_single_click_subscribe(BUTTON_ID_SELECT, prv_select_click_handler);
//   window_single_click_subscribe(BUTTON_ID_UP, prv_up_click_handler);
//   window_single_click_subscribe(BUTTON_ID_DOWN, prv_down_click_handler);
// }



// Save the settings to persistent storage
// static void prv_save_settings() {
//   save_settings();
//   // Update the display based on new settings
//   prv_update_display();
// }

static void prv_update_display() {
  //update_time();
}

static void prv_periodic_vibrate(struct tm *timer) {
  if (timer->tm_sec % 10 == 0) {
  if (settings_get_VibratePeriodic() && can_vibrate()) {
    do_vibrate(settings_get_VibratePeriodicType());
  }
}
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  //update_time();
  prv_periodic_vibrate(tick_time);

  window_update_time();
//  change_pic(tick_time->tm_min);
  if (tick_time->tm_min % 15 == 0) {
    //update_weather();
  }
}




void settings_update_handler(UPDATE_FLAG f) {
  if (f == UF_WEATHER) {
//    init_weather(settings_get_weather_apikey());
  }
  prv_update_display();
}

static void prv_inbox_received_handler(DictionaryIterator *iter, void *context) {
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

static void prv_init() {
  init_settings(settings_update_handler);

  app_message_register_inbox_received(prv_inbox_received_handler);
  app_message_open(128, 128);
  init_time_window();

//  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
//  update_time();
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
