#include <pebble.h>
#include "include/weather_m.h"
#include "../utils/include/timeutils.h"
#include "../utils/include/messagesystem.h"
#include "../settings.h"
#include "../3rdparty/locale_framework/localize.h"

//static const uint8_t max_attempts = 5;

//static AppTimer *s_timeout_timer;
static Layer *s_this_layer;
static GFont s_wfont;
static void prv_populate_weather_layer(Layer *, GContext *);
static void prv_save_weather();
static void prv_load_weather();
// static void outbox_sent_handler(DictionaryIterator *, void *);
// static void send_with_timeout(uint_8, int, uint8_t );
static void prv_send_data_failed();
static void prv_timer_timeout_handler(void*);
static AppTimer *s_timeout_timer;
static const int timeout = 5000;


typedef struct WeatherData {
  uint8_t WeatherReady;
  int WeatherTemperature;
  char WeatherCondition[4];
  char WeatherDesc[32];
  uint32_t WeatherTimeStamp;
  int WeatherPressure;
  uint8_t WeatherWindSpeed;
  char WeatherWindDirection[4];
  uint8_t WeatherHumidity;
  char WeatherSunrise[8];
  char WeatherSunset[8];  
} __attribute__((__packed__)) WeatherData;

static WeatherData weather;

void prv_default_weather_data() {
  weather.WeatherReady = 0;
  weather.WeatherTimeStamp = 0;
//  weather.WeatherCondition = 'h';
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

void update_weather(bool force) {
  int secs_to_wait = period_to_mins(settings_get_WeatherUpdatePeriod()) * SECONDS_PER_MINUTE;
  uint32_t elapsed = weather.WeatherTimeStamp + secs_to_wait;
    if (!force) {
     if (elapsed > (uint32_t)time(NULL)) {
      APP_LOG(APP_LOG_LEVEL_DEBUG, "No weather update needed, update in %ld", (uint32_t)time(NULL) - elapsed);
      return;
    }
  }
//  APP_LOG(APP_LOG_LEVEL_DEBUG, "Let's get weather!, %ld", (time(NULL)-elapsed)/60);
  //send_message(MESSAGE_KEY_WeatherMarker, 1);
  // Begin dictionary
//return ;
  Tuplet data_to_send[] = {
    TupletInteger(MESSAGE_KEY_WeatherMarker, 1),
  };
  send_message(data_to_send, 1, prv_send_data_failed);
}

void prv_populate_weather_layer(Layer *me, GContext *ctx) {
  if (weather.WeatherReady == 1) {

  static char weather_text[8];

  snprintf(weather_text, sizeof(weather_text), "%d°", weather.WeatherTemperature);
  graphics_draw_text(ctx, weather.WeatherCondition, \
    s_wfont, \
    GRect (4, 0, 30, 30), \
    GTextOverflowModeWordWrap, \
    GTextAlignmentCenter, \
    NULL);        
  graphics_draw_text(ctx, weather_text, \
    fonts_get_system_font(FONT_KEY_GOTHIC_24), \
    GRect(0, 28, 43, 20), \
    GTextOverflowModeWordWrap, \
    GTextAlignmentCenter, \
    NULL);        
}
}

void get_weather(DictionaryIterator *iter, void *context) {
//  APP_LOG(APP_LOG_LEVEL_DEBUG, "parsing weather_text");

  Tuple *w_temp = dict_find(iter, MESSAGE_KEY_WeatherTemperature);
  if (w_temp) {
  //  static char w_tempbuffer[4];
    weather.WeatherTemperature = w_temp->value->int32;
    weather.WeatherReady = 1;
  }

  Tuple *w_desc = dict_find(iter, MESSAGE_KEY_WeatherDesc);
  if (w_desc) {
    snprintf(weather.WeatherDesc, sizeof(weather.WeatherDesc), w_desc->value->cstring);
  }
  
  Tuple *w_cond = dict_find(iter, MESSAGE_KEY_WeatherCondition);
  if (w_cond) {
    snprintf(weather.WeatherCondition, sizeof(weather.WeatherCondition), w_cond->value->cstring);
  }

  weather.WeatherTimeStamp = time(NULL);
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "weather timestamp: %ld", weather.WeatherTimeStamp);

  Tuple *w_press = dict_find(iter, MESSAGE_KEY_WeatherPressure);
  if (w_press) {
    weather.WeatherPressure = w_press->value->int32;
  }

  Tuple *w_ws = dict_find(iter, MESSAGE_KEY_WeatherWindSpeed);
  if (w_ws) {
    weather.WeatherWindSpeed = w_ws->value->uint8;
  }

  Tuple *w_hum = dict_find(iter, MESSAGE_KEY_WeatherHumidity);
  if (w_hum) {
    weather.WeatherHumidity = w_hum->value->uint8;
  }

  Tuple *w_wdir = dict_find(iter, MESSAGE_KEY_WeatherWindDirection);
  if (w_wdir) {
    snprintf(weather.WeatherWindDirection, sizeof(weather.WeatherWindDirection), w_wdir->value->cstring);
//    weather.WeatherWindDirection = w_wdir->value->int32;
  }

  Tuple *w_rise = dict_find(iter, MESSAGE_KEY_WeatherSunrise);
  if (w_rise) {
    snprintf(weather.WeatherSunrise, sizeof(weather.WeatherSunrise), w_rise->value->cstring);
//    weather.WeatherWindDirection = w_wdir->value->int32;
  }
  Tuple *w_set = dict_find(iter, MESSAGE_KEY_WeatherSunset);
  if (w_set) {
    snprintf(weather.WeatherSunset, sizeof(weather.WeatherSunset), w_set->value->cstring);
//    weather.WeatherWindDirection = w_wdir->value->int32;
  }  
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

uint8_t get_weatherIsReady() {
  return weather.WeatherReady;
}

int get_WeatherTemperature() {
  return weather.WeatherTemperature;
}

char* get_WeatherCondition() {
  return weather.WeatherCondition;
}

char* get_WeatherDesc() {
  return weather.WeatherDesc;
}

char* get_WeatherSunrise() {
  return weather.WeatherSunrise;
}

char* get_WeatherSunset() {
  return weather.WeatherSunset;
}

uint32_t get_WeatherTimeStamp() {
  return weather.WeatherTimeStamp;
}

int get_WeatherPressure() {
  return weather.WeatherPressure;
}

uint8_t get_WeatherWindSpeed() {
  return weather.WeatherWindSpeed;
}

char* get_WeatherWindDirection() {
  return weather.WeatherWindDirection;
}

uint8_t get_WeatherHumidity() {
  return weather.WeatherHumidity;
}

static void prv_send_data_failed() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Weather send message failed, timeput 5 secs");
  s_timeout_timer = app_timer_register(timeout, prv_timer_timeout_handler, NULL);
}

static void prv_timer_timeout_handler (void *context) {
  update_weather(false);
}