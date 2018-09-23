#include <pebble.h>
#include "include/forecast-window.h"
#include "../modules/include/weather_full_m.h"
#include "../modules/include/forecast_m.h"
#include "../settings.h"

static Window *s_forecast_window;
static void prv_populate_fc_layer(Layer *, GContext *);

Window* get_forecast_window() {
  #if defined (DEBUG)
     APP_LOG(APP_LOG_LEVEL_DEBUG, "Request forecast window");
  #endif
 
  if (!s_forecast_window) {
    init_forecast_window();
  }
  return s_forecast_window;
}

void deinit_forecast_window() {
  window_destroy(s_forecast_window);
}

static void prv_window_load(Window *window) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "forecast Window loading");
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  GRect weather_full_rect = GRect (0, 0, bounds.size.w, 75);
  GRect forecast_rect = GRect (0, 76, bounds.size.w, bounds.size.h - 76);


  init_weather_full_layer(weather_full_rect);
  init_forecast_layer(forecast_rect);

  layer_add_child(window_layer, get_layer_weather_full());
  layer_add_child(window_layer, get_layer_forecast());
}

static void prv_window_unload(Window *window) {
  deinit_weather_full_layer();
  deinit_forecast_layer();  
}

static void prv_populate_fc_layer(Layer *me, GContext *ctx) {
  GRect this_rect = layer_get_bounds(me);
  settings_get_theme(ctx);
  graphics_fill_rect(ctx, this_rect, 0, GCornerNone);  
};


void init_forecast_window() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "forecast Window init");
  s_forecast_window = window_create();
  window_set_window_handlers(s_forecast_window, (WindowHandlers) {
    .load = prv_window_load,
    .unload = prv_window_unload,
  });
  layer_set_update_proc(window_get_root_layer(s_forecast_window), prv_populate_fc_layer);
}

void ready_for_forecast(bool force) {
  #if defined (DEBUG)
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Ready for forecast");
  #endif
  
  update_forecast(force);
}

void forecast_update_disp(DictionaryIterator *iter, void *context) {
  forecast_update(iter, context);
}