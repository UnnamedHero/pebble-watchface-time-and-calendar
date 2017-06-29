#include <pebble.h>
#include "twc-ng.h"
//#include "localize.h"
#include "settings.h"
#include "weather.h"

static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_weather_layer;
static bool s_js_ready;

//---------bitmaps
static BitmapLayer *s_top_panel_layer_1;

static GBitmap *s_bt_connected;


//---------functions
static void update_time();

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
  update_time();
}


static void tick_handler(struct tm *tick_time, TimeUnits units_chanhed) {
  update_time();

  if (tick_time->tm_min % 15 == 0) {
    update_weather();
  }
}

static void prv_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

//-----bitmaps
  s_bt_connected = gbitmap_create_with_resource(RESOURCE_ID_BLUETOOTH_CONNECTED);

//----bitmap layers
  s_top_panel_layer_1 = bitmap_layer_create((GRect(0, 0, 20, 20)));

//----text layers
  s_time_layer = text_layer_create(GRect(0, 52, bounds.size.w, 20));
  s_weather_layer = text_layer_create(GRect(0, 72, bounds.size.w, 20));
//----set bitmaps
  bitmap_layer_set_bitmap(s_top_panel_layer_1, s_bt_connected);

//----set text
  text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  text_layer_set_text(s_weather_layer, "n/a");
  text_layer_set_text_alignment(s_weather_layer, GTextAlignmentCenter);


//---adding layers
  layer_add_child(window_layer, bitmap_layer_get_layer(s_top_panel_layer_1));
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_weather_layer));
  prv_update_display();
}

static void prv_window_unload(Window *window) {
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_weather_layer);
  gbitmap_destroy(s_bt_connected);
  bitmap_layer_destroy(s_top_panel_layer_1);
}

void settings_update_handler(UPDATE_FLAG f) {
  if (f == UF_WEATHER) {
    init_weather(settings_get_weather_apikey());
  }
  prv_update_display();
}

static void prv_inbox_received_handler(DictionaryIterator *iter, void *context) {
  Tuple *js_ready_t = dict_find(iter, MESSAGE_KEY_JSReady);
  if (js_ready_t) {
    s_js_ready = true;
    init_weather(settings_get_weather_apikey());
  }

  Tuple *config_marker = dict_find(iter, MESSAGE_KEY_ConfigMarker);
  if (config_marker) {
    populate_settings(iter, context);
  }

  Tuple *weather_marker = dict_find(iter, MESSAGE_KEY_WeatherMarker);
  if (weather_marker) {
    Tuple *w_temp = dict_find(iter, MESSAGE_KEY_WeatherTemperature);
    if (w_temp) {
      static char w_tempbuffer[4];
      snprintf(w_tempbuffer, sizeof(w_tempbuffer), "%dC", (int)w_temp->value->int32);
      text_layer_set_text(s_weather_layer, w_tempbuffer);
    }
  }

}

static void prv_init() {
  //locale_init();
  // int (*handler_ptr)(void) = NULL;
  // handler_ptr = &settings_update_handler;
  // init_settings(handler_ptr);
  init_settings(settings_update_handler);

  app_message_register_inbox_received(prv_inbox_received_handler);
  app_message_open(128, 128);

  s_main_window = window_create();
//  window_set_click_config_provider(s_window, prv_click_config_provider);
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = prv_window_load,
    .unload = prv_window_unload,
  });
  const bool animated = true;

  // Open AppMessage connection


  window_stack_push(s_main_window, animated);
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  update_time();
}

static void prv_deinit(void) {
  window_destroy(s_main_window);
}

int main(void) {
  prv_init();
  app_event_loop();
  prv_deinit();
}

static void update_time() {
    time_t temp = time(NULL);
    struct tm *tick_time = localtime(&temp);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "time format:[%s]", settings_get_clockformat());
    static char s_buffer[8];//    char format[5];
    strftime(s_buffer, sizeof(s_buffer), settings_get_clockformat(), tick_time);
    text_layer_set_text(s_time_layer, s_buffer);
}
