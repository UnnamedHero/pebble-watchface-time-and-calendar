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
//static TextLayer *s_weather_layer;

//static void prv_update_window();

Window* get_time_window() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Request time window");
  if (!s_time_window) {
    init_time_window();
  }
  return s_time_window;
}

void deinit_time_window() {
  window_destroy(s_time_window);
}

static void prv_window_load(Window *window) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Time Window loading");
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  window_set_background_color(window, GColorBlack);
//-----bitmaps
//  GRect toplayer_bounds = GRect (0, 0, bounds.size.w, 20);
  GRect bluetooth_bounds = GRect (0, 0, 20, 20);
  GRect battery_bounds = GRect (bounds.size.w - 47, 0, 45, 20);
  GRect date_bounds = GRect(0, 22, bounds.size.w, 14);
  //GRect datetime_bounds = GRect (0, 20, bounds.size.w, 60);
  GRect time_bounds = GRect (35, 32, bounds.size.w - 35, 54);
  GRect calendar_bounds = GRect (2, 92, bounds.size.w, 73);
//  GRect weather_bounds = GRect (2, 138, bounds.size.w, bounds.size.h - 123);
  GRect weather_bounds = GRect (2, 34, 34, 54);
//  init_top_panel_layer(toplayer_bounds);
  init_bluetooh_layer(bluetooth_bounds);
  init_battery_layer(battery_bounds);
  init_date_layer(date_bounds);
  init_time_layer(time_bounds);
  //init_datetime_layer(datetime_bounds);
  init_calendar_layer(calendar_bounds);
  init_weather_layer(weather_bounds);

//---adding layers
  layer_add_child(window_layer, get_layer_bluetooth());
  layer_add_child(window_layer, get_layer_battery());
  layer_add_child(window_layer, get_layer_date());
  layer_add_child(window_layer, get_layer_time());
  layer_add_child(window_layer, get_layer_calendar());
  layer_add_child(window_layer, get_layer_weather());
}

static void prv_window_unload(Window *window) {
  deinit_bluetooth_layer();
  deinit_battery_layer();
  deinit_date_layer();
  deinit_time_layer();
  deinit_calendar_layer();
  deinit_weather_layer();
}


void init_time_window() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Time Window init");
  s_time_window = window_create();
//  window_set_click_config_provider(s_window, prv_click_config_provider);
  window_set_window_handlers(s_time_window, (WindowHandlers) {
    .load = prv_window_load,
    .unload = prv_window_unload,
  });
}

void window_update_time(struct tm *tick_time) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Time Window update time handler");

  layer_mark_dirty(get_layer_time());
//  update_weather();
}


void ready_for_weather() {
  update_weather();
}

void simple_weather_update(DictionaryIterator *iter, void *context) {
  get_weather(iter, context);
}

void time_window_force_redraw() {
  layer_mark_dirty(get_layer_bluetooth());
  layer_mark_dirty(get_layer_battery());
  layer_mark_dirty(get_layer_date());
  layer_mark_dirty(get_layer_time());
  layer_mark_dirty(get_layer_calendar());
  layer_mark_dirty(get_layer_weather());
}