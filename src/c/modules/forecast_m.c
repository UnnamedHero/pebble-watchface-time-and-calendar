#include <pebble.h>
#include "include/forecast_m.h"
#include "../settings.h"
#include "../utils/include/messagesystem.h"

static Layer *s_forecast_layer;
static GFont s_wfont_sm;
static void prv_populate_forecast_layer(Layer *, GContext *);
static void prv_send_data_failed();
static void prv_timer_timeout_handler(void*);
static void prv_load_forecast();
static void prv_save_forecast();

static AppTimer *s_timeout_timer;
static const int timeout = 5000;

typedef struct ForecastData {
  uint8_t ForecastReady;
  uint8_t ForecastQty;
  uint32_t ForecastTime;
  int ForecastTemperature[5];
  char ForecastCondition[5][4];
  char ForecastTimeStamp[5][16];
} __attribute__((__packed__)) ForecastData;

static ForecastData forecast;

void init_forecast_layer(GRect rect) {
  s_forecast_layer = layer_create(rect);
  s_wfont_sm = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_CLIMACONS_36));
  prv_load_forecast();
  layer_set_update_proc(s_forecast_layer, prv_populate_forecast_layer);
}

void prv_default_forecast_data() {
  forecast.ForecastTime = 1;
  forecast.ForecastReady = 0;
  forecast.ForecastQty = 0;
//  weather.WeatherCondition = 'h';
}

void deinit_forecast_layer() {
	layer_destroy(s_forecast_layer);
  fonts_unload_custom_font(s_wfont_sm);
}

Layer* get_layer_forecast() {
	return s_forecast_layer;
}


static void prv_populate_forecast_layer(Layer *me, GContext *ctx) {	
  layer_set_hidden(me, true);
  layer_set_clips(me, false);

  if (forecast.ForecastReady != 1 || forecast.ForecastQty == 0) {
    return;
  }

  APP_LOG(APP_LOG_LEVEL_DEBUG, "DRAW forecast layer");  
  GRect bounds = layer_get_bounds(me);
  int item_width = bounds.size.w / forecast.ForecastQty;
  int item_height = bounds.size.h;
  graphics_context_set_stroke_color(ctx, GColorWhite);
  for (int i = 0; i < forecast.ForecastQty; i++) {
    const int module_x = i * item_width + 2;
    const int module_y = 0;
//    GRect module_bounds = GRect(module_x, module_y, item_width, item_height);

    const GRect dt = GRect(module_x, module_y, item_width, item_height);
    const GRect cond = GRect (module_x, module_y + 32, item_width, 20);
  //  graphics_draw_rect(ctx, module_bounds);
    static char forec_text[64];

    snprintf(forec_text, sizeof(forec_text), \
      "%s\n\n\n%d°",
      forecast.ForecastTimeStamp[i],\
      forecast.ForecastTemperature[i]\
      );

    graphics_draw_text(ctx, forec_text, \
    fonts_get_system_font(FONT_KEY_GOTHIC_18), \
    dt, \
    GTextOverflowModeWordWrap, \
    GTextAlignmentCenter, \
    NULL);   

    graphics_draw_text(ctx, forecast.ForecastCondition[i], \
    s_wfont_sm, \
    cond, \
    GTextOverflowModeWordWrap, \
    GTextAlignmentCenter, \
    NULL);   
  }

  layer_set_clips(me, true);
  layer_set_hidden(me, false);
}

void forecast_update(DictionaryIterator *iter, void *context) {
  // bool forecast_ready = false;
  // int forecast_qty = 0;

  Tuple *f_mark = dict_find(iter, MESSAGE_KEY_WeatherMarkerForecast);
   if (f_mark) {
      //forecast_ready = f_mark->value->uint8 == 1;
      forecast.ForecastReady = f_mark->value->uint8;
   }

  Tuple *f_qty = dict_find(iter, MESSAGE_KEY_ForecastQty);
   if (f_qty) {
//      forecast_qty = f_qty->value->uint8;
      forecast.ForecastQty = f_qty->value->uint8;
   }
//   APP_LOG(APP_LOG_LEVEL_DEBUG, "ready %d, qty %d", f_mark->value->uint8, forecast.ForecastQty);
   if (forecast.ForecastReady != 1 || forecast.ForecastQty == 0) {
      return;
   }
   APP_LOG(APP_LOG_LEVEL_DEBUG, "Start filling forecast");
   Tuple *f_time = dict_find(iter, MESSAGE_KEY_ForecastTime);
   if (f_time) {
      forecast.ForecastTime = f_time->value->int32;
   }       

   for (int i = 0; i < forecast.ForecastQty; i++) {    
      Tuple *f_temp = dict_find(iter, MESSAGE_KEY_ForecastTemperature + i);
      if (f_temp) {
        forecast.ForecastTemperature[i] = f_temp->value->int32;
       }

      // Tuple *f_tempmin = dict_find(iter, MESSAGE_KEY_ForecastTemperatureMin + i);
      // if (f_tempmin) {
      //   forecast.ForecastTemperatureMin[i] = f_tempmin->value->int32;
      //  }

      // Tuple *f_tempmax = dict_find(iter, MESSAGE_KEY_ForecastTemperatureMax + i);
      // if (f_tempmax) {
      //   forecast.ForecastTemperatureMax[i] = f_tempmax->value->int32;
      //  }       


      Tuple *f_cond = dict_find(iter, MESSAGE_KEY_ForecastCondition + i);
      if (f_cond) {
        snprintf(forecast.ForecastCondition[i], sizeof(forecast.ForecastCondition[i]), f_cond->value->cstring);
      }

      Tuple *f_ts = dict_find(iter, MESSAGE_KEY_ForecastTimeStamp + i);
      if (f_ts) {
        snprintf(forecast.ForecastTimeStamp[i], sizeof(forecast.ForecastTimeStamp[i]), f_ts->value->cstring);
      }      
   }
   APP_LOG(APP_LOG_LEVEL_DEBUG, "tuple done");
   prv_save_forecast();
   if (s_forecast_layer) {
     layer_mark_dirty(s_forecast_layer);
    }
}

void update_forecast(bool force) {
  if (!s_forecast_layer) {
    prv_load_forecast();
  }

  if (!force) {
    if ((uint32_t)time(NULL) < forecast.ForecastTime) {
      APP_LOG(APP_LOG_LEVEL_DEBUG, "%ld < %ld", (uint32_t)time(NULL), forecast.ForecastTime);  
      APP_LOG(APP_LOG_LEVEL_DEBUG, "No forecast update needed");
      return;
    }

  }

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Let's get forecast!");

    Tuplet data_to_send[] = {
    TupletInteger(MESSAGE_KEY_WeatherMarkerForecast, 1),
  };
  send_message(data_to_send, 1, prv_send_data_failed);
}

static void prv_send_data_failed() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Forecast send message failed, timeput 5 secs");
  s_timeout_timer = app_timer_register(timeout, prv_timer_timeout_handler, NULL);
}

static void prv_timer_timeout_handler (void *context) {
  update_forecast(false);
}

static void prv_save_forecast() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "save:%d", persist_write_data(FORECAST_KEY, &forecast, sizeof(forecast)));
}

static void prv_load_forecast() {
  prv_default_forecast_data();
  persist_read_data(FORECAST_KEY, &forecast, sizeof(forecast));
}
