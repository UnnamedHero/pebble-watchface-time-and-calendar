#include <pebble.h>
#include "include/weather_m.h"
#include "../utils/include/timeutils.h"
#include "../settings.h"
#include "../3rdparty/locale_framework/localize.h"

static Layer *s_this_layer;
static GFont s_wfont;
static void prv_populate_weather_layer(Layer *, GContext *);
static void prv_save_weather();
static void prv_load_weather();

typedef struct WeatherData {
  uint8_t WeatherReady;
  int WeatherTemperature;
  char WeatherCondition[4];
//  char WeatherDesc[32];
  uint32_t WeatherTimeStamp;
  // int WeatherPressure;
  // uint8_t WeatherWindSpeed;
  // int WeatherWindDirection;
} __attribute__((__packed__)) WeatherData;

static WeatherData weather;

void prv_default_weather_data() {
  weather.WeatherReady = 0;
  weather.WeatherTimeStamp = 0;
//  weather.WeatherCondition = 'h';
}

int prv_get_wind_direction(int azim) {
  int normalized = azim + 11;
  int azimuth = normalized > 360 ? normalized - 360 : normalized;
  return (int)(azimuth / 22.5);
}

void init_weather_layer(GRect bounds) {  
  s_this_layer = layer_create(bounds);
  s_wfont = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_CLIMACONS_36));
  prv_load_weather();
  layer_set_update_proc(s_this_layer, prv_populate_weather_layer);
}

void deinit_weather_layer() {
  if (s_this_layer) {
    layer_destroy(s_this_layer);
  }
  fonts_unload_custom_font(s_wfont);
}

Layer* get_layer_weather() {
  return s_this_layer;
}

void update_weather() {
  int secs_to_wait = period_to_mins(settings_get_WeatherUpdatePeriod()) * SECONDS_PER_MINUTE;
  uint32_t elapsed = weather.WeatherTimeStamp + secs_to_wait;
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "sav: %ld, per: %d, el: %ld, cur: %ld",weather.WeatherTimeStamp, secs_to_wait, elapsed, time(NULL));
  if (elapsed > (uint32_t)time(NULL)) {
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "No weather update needed");
  //  return;
  }
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "Let's get weather!, %ld", (time(NULL)-elapsed)/60);

  // Begin dictionary
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

  // Add a key-value pair
  dict_write_uint8(iter, MESSAGE_KEY_WeatherMarker, 0);

  // Send the message!
  app_message_outbox_send();
}

void prv_populate_weather_layer(Layer *me, GContext *ctx) {
  /*
  char* wind_directions[] = {_("N"), _("NNE"), _("NE"), _("ENE"), _("E"), _("ESE"), _("SE"), _("SSE"), _("S"), _("SSW"), _("SW"), _("WSW"), _("W"), _("WNW"), _("NW"), _("NNW")};
  
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Draw: WEATHER");
    GRect this_bounds = layer_get_bounds(s_this_layer);
    //int wind_direction = prv_get_wind_direction()
    static char weather_text[64];
//    APP_LOG(APP_LOG_LEVEL_DEBUG, "az is %d, wind is %d", weather.WeatherWindDirection, prv_get_wind_direction(weather.WeatherWindDirection));
    snprintf(weather_text, sizeof(weather_text), "%d°, %s,\n%s, %d %s, %d %s", \
        weather.WeatherTemperature,\
        weather.WeatherDesc,\
        wind_directions[prv_get_wind_direction(weather.WeatherWindDirection)],\
        weather.WeatherWindSpeed, \
        _("m/s"), \
        (int)(weather.WeatherPressure * 0.75), \
        _("mmHg") \
      );
    graphics_draw_text(ctx, weather_text, \
        fonts_get_system_font(FONT_KEY_GOTHIC_14), \
        this_bounds, \
        GTextOverflowModeWordWrap, \
        GTextAlignmentLeft, \
        NULL);
  }
*/
  if (weather.WeatherReady == 1) {
  static char weather_text[64];
  APP_LOG(APP_LOG_LEVEL_DEBUG, "temp is %d", weather.WeatherTemperature);  
  snprintf(weather_text, sizeof(weather_text), "%d°", weather.WeatherTemperature);
  graphics_draw_text(ctx, weather.WeatherCondition, \
    s_wfont, \
    GRect (4, 0, 30, 30), \
    GTextOverflowModeWordWrap, \
    GTextAlignmentLeft, \
    NULL);        
  graphics_draw_text(ctx, weather_text, \
    fonts_get_system_font(FONT_KEY_GOTHIC_24), \
    GRect(4, 28, 30, 20), \
    GTextOverflowModeWordWrap, \
    GTextAlignmentLeft, \
    NULL);        
}
}

void get_weather(DictionaryIterator *iter, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "parsing weather_text");

  Tuple *w_temp = dict_find(iter, MESSAGE_KEY_WeatherTemperature);
  if (w_temp) {
  //  static char w_tempbuffer[4];
    weather.WeatherTemperature = w_temp->value->int32;
    weather.WeatherReady = 1;
  }

  // Tuple *w_desc = dict_find(iter, MESSAGE_KEY_WeatherDesc);
  // if (w_desc) {
  //   snprintf(weather.WeatherDesc, sizeof(weather.WeatherDesc), w_desc->value->cstring);
  // }
  Tuple *w_cond = dict_find(iter, MESSAGE_KEY_WeatherCondition);
  if (w_cond) {
    snprintf(weather.WeatherCondition, sizeof(weather.WeatherCondition), w_cond->value->cstring);
  }

  weather.WeatherTimeStamp = time(NULL);
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "weather timestamp: %ld", weather.WeatherTimeStamp);

  // Tuple *w_press = dict_find(iter, MESSAGE_KEY_WeatherPressure);
  // if (w_press) {
  //   weather.WeatherPressure = w_press->value->int32;
  // }

  // Tuple *w_ws = dict_find(iter, MESSAGE_KEY_WeatherWindSpeed);
  // if (w_ws) {
  //   weather.WeatherWindSpeed = w_ws->value->uint8;
  // }

  // Tuple *w_wdir = dict_find(iter, MESSAGE_KEY_WeatherWindDirection);
  // if (w_wdir) {
  //   weather.WeatherWindDirection = w_wdir->value->int32;
  // }
  prv_save_weather();
  layer_mark_dirty(s_this_layer);
}
static void prv_save_weather() {
    persist_write_data(WEATHER_KEY, &weather, sizeof(weather));
}

static void prv_load_weather() {
  prv_default_weather_data();
  persist_read_data(WEATHER_KEY, &weather, sizeof(weather));
}
