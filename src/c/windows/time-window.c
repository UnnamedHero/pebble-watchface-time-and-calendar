#include <pebble.h>
#include "time-window.h"
//#include "../layers/top-panel-layer.h"
//#include "../layers/include/datetime-layer.h"
#include "../modules/include/calendar_m.h"
#include "../modules/include/bluetooth_m.h"
#include "../modules/include/battery_m.h"
#include "../modules/include/date_m.h"
#include "../modules/include/time_m.h"
#include "../modules/include/weather_m.h"
#include "../settings.h"

static Window *s_time_window;

//static TextLayer *s_time_layer;
static TextLayer *s_weather_layer;

//static void prv_update_window();

Window* get_time_window() {
  if (!s_time_window) {
    init_time_window();
  }
  return s_time_window;
}

void deinit_time_window() {
  window_destroy(s_time_window);
}

static void prv_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  window_set_background_color(window, GColorBlack);
//-----bitmaps
//  GRect toplayer_bounds = GRect (0, 0, bounds.size.w, 20);
  GRect bluetooth_bounds = GRect (0, 0, 20, 20);
  GRect battery_bounds = GRect (bounds.size.w - 45, 0, 45, 20);
  GRect date_bounds = GRect(0, 13, bounds.size.w, 24);
  //GRect datetime_bounds = GRect (0, 20, bounds.size.w, 60);
  GRect time_bounds = GRect (0, 28, bounds.size.w, 42);
  GRect calendar_bounds = GRect (2, 70, bounds.size.w, 73);
  GRect weather_bounds = GRect (0, 143, bounds.size.w, bounds.size.h - 123);
//  init_top_panel_layer(toplayer_bounds);
  init_bluetooh_layer(bluetooth_bounds);
  init_battery_layer(battery_bounds);
  init_date_layer(date_bounds);
  init_time_layer(time_bounds);
  //init_datetime_layer(datetime_bounds);
  init_calendar_layer(calendar_bounds);
  init_weather_layer(weather_bounds);
//----bitmap layers

  //bitmap_layer_create((GRect(0, 0, bounds.size.w, 20)));


//----text layers
//  s_time_layer = text_layer_create(GRect(0, 52, bounds.size.w, 20));
  s_weather_layer = text_layer_create(GRect(0, 72, bounds.size.w, 20));
//----set bitmaps
//  bitmap_layer_set_bitmap(s_top_panel_layer_1, s_bt_connected);
//----set text
  // text_layer_set_text(s_time_layer, "00:00");
  // text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  //text_layer_set_text(s_weather_layer, "n/a");
  //text_layer_set_text_alignment(s_weather_layer, GTextAlignmentCenter);


//---adding layers
//  layer_add_child(window_layer, get_top_panel_layer());
  layer_add_child(window_layer, get_layer_bluetooth());
  layer_add_child(window_layer, get_layer_battery());
  layer_add_child(window_layer, get_layer_date());
  layer_add_child(window_layer, get_layer_time());
  //layer_add_child(window_layer, get_datetime_layer());
  layer_add_child(window_layer, get_layer_calendar());
  layer_add_child(window_layer, get_layer_weather());
//  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  //layer_add_child(window_layer, text_layer_get_layer(s_weather_layer));
//  prv_update_window();
}

static void prv_window_unload(Window *window) {
//  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_weather_layer);
  //layer_destroy(s_top_panel_layer_1);
//  deinit_top_panel_layer();
  deinit_bluetooth_layer();
  deinit_battery_layer();
  deinit_date_layer();
  deinit_time_layer();
  deinit_calendar_layer();
  deinit_weather_layer();
}


void init_time_window() {
  s_time_window = window_create();
//  window_set_click_config_provider(s_window, prv_click_config_provider);
  window_set_window_handlers(s_time_window, (WindowHandlers) {
    .load = prv_window_load,
    .unload = prv_window_unload,
  });
  const bool animated = true;

  // Open AppMessage connection

  window_stack_push(s_time_window, animated);
}

// static void prv_update_window() {
//   update_time();
// }

void window_update_time() {
  //layer_update_time();
  time_layer_update_time();
}

// void update_time() {
//     time_t temp = time(NULL);
//     struct tm *tick_time = localtime(&temp);
//     APP_LOG(APP_LOG_LEVEL_DEBUG, "time format:[%s]", settings_get_clockformat());
//     static char s_buffer[8];//    char format[5];
//     strftime(s_buffer, sizeof(s_buffer), settings_get_clockformat(), tick_time);
//     text_layer_set_text(s_time_layer, s_buffer);
// }

void ready_for_weather() {
  update_weather();
}

void simple_weather_update(DictionaryIterator *iter, void *context) {
  get_weather(iter, context);
}
