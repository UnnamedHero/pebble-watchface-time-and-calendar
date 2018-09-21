#include <pebble.h>
#include "include/weather_m.h"
#include "../utils/include/timeutils.h"
#include "../utils/include/messagesystem.h"
#include "../utils/include/ticktimerhelper.h"
#include "../settings.h"
#include "../3rdparty/locale_framework/localize.h"

static Layer *s_this_layer;
static GFont s_wfont;
static GFont s_tfont, s_tdigfont;
static GFont s_tfont_sec, s_tdigitfont_sec;
static void prv_populate_this_layer(Layer *, GContext *);
static void prv_populate_combined_layer(Layer *, GContext *);
static void prv_populate_time_layer(Layer *, GContext *, GRect);
static void prv_save_weather();
static void prv_load_weather();
static void prv_send_data_failed();
static void prv_timer_timeout_handler(void*);
static void prv_ticktimer(struct tm*);
static void prv_send_weather_update_request();

static AppTimer *s_timeout_timer;
static const int timeout = 5000;
static bool is_force_update;
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
  if (weather.WeatherReady != 1) {
    weather.WeatherReady = 0;
  }

  helper_str_filler(weather.WeatherCondition, "h");
}

void init_weather_layer(GRect bounds) {  
  is_force_update = false;
  s_this_layer = layer_create(bounds);
  s_wfont = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_CLIMACONS_36));
  s_tfont = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_TIME_BOLD_58));
  s_tdigfont = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_DIGITAL_BOLD_46));
  s_tfont_sec = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_TIME_SECS_BOLD_44));
  s_tdigitfont_sec = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_DIGITAL_SECS_BOLD_42));
  prv_load_weather();
  layer_set_update_proc(s_this_layer, prv_populate_this_layer);
  ticktimerhelper_register(prv_ticktimer);
}

void deinit_weather_layer() {
  if (s_this_layer) {
    layer_destroy(s_this_layer);
  }
  fonts_unload_custom_font(s_wfont);
  fonts_unload_custom_font(s_tfont);
  fonts_unload_custom_font(s_tdigfont);  
  fonts_unload_custom_font(s_tfont_sec);
  fonts_unload_custom_font(s_tdigitfont_sec);  
}

Layer* get_layer_weather() {
  return s_this_layer;
}

static void prv_ticktimer(struct tm* unneeded) {
  update_weather(false);
}

void update_weather(bool force) {
  is_force_update = force;

  #if defined (DEBUG)
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Update weather, force: %s, status: %i", force ? "true" : "false", settings_get_WeatherStatus());
  #endif
  
  if (force) {
    prv_send_weather_update_request();
    return;
  }

  if (!can_update_weather()) {
    return;
  }

  if (is_time_to(weather.WeatherTimeStamp, settings_get_WeatherUpdatePeriod())) {
    prv_send_weather_update_request();
    return;
  } 

  #if defined (DEBUG) 
    APP_LOG(APP_LOG_LEVEL_DEBUG, "No weather update needed");
  #endif  
}

void prv_send_weather_update_request() {
  #if defined (DEBUG) 
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Request weather");
  #endif

  Tuplet data_to_send[] = {
    TupletInteger(MESSAGE_KEY_WeatherMarker, 1),
  };
  send_message(data_to_send, ARRAY_LENGTH(data_to_send), prv_send_data_failed);
}

void prv_populate_this_layer(Layer *me, GContext *ctx) {
  settings_get_theme(ctx);

  GRect bounds = layer_get_bounds(me);
  if (settings_get_ClockShowSeconds() == SEC_SHOWING) {
    prv_populate_time_layer(me, ctx, bounds);
    return;
  }

  weather.WeatherReady == 1 && (settings_get_WeatherStatus() == WEATHER_OK || \
  settings_get_WeatherStatus() == WEATHER_LOCATION_ERROR) ? \
    prv_populate_combined_layer (me, ctx) : prv_populate_time_layer(me, ctx, bounds);
}


void prv_populate_combined_layer(Layer *me, GContext *ctx) {

  #if defined (DEBUG) 
    APP_LOG(APP_LOG_LEVEL_DEBUG, "DRAW small weather");
  #endif
  GRect bounds = layer_get_bounds(me);
  static const int offset = 10;
  int weather_width = bounds.size.w / 3 - offset;
  const GRect combined = GRect (weather_width, 0, bounds.size.w - weather_width, bounds.size.h);
  prv_populate_time_layer(me, ctx, combined);

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

static GFont prv_get_time_font() {
  switch (settings_get_TimeFont()) {
    case TF_BEBAS:
      return settings_get_ClockShowSeconds() == SEC_SHOWING ? s_tfont_sec : s_tfont;
    case TF_DIGITAL:
      return settings_get_ClockShowSeconds() == SEC_SHOWING ? s_tdigitfont_sec : s_tdigfont;      
  }
  return s_tfont;
}

static void prv_populate_time_layer(Layer *me, GContext *ctx, GRect rect) {
  #if defined (DEBUG) 
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Draw: TIME");
  #endif

  static char time_txt[33];
  settings_get_ClockShowSeconds() == SEC_SHOWING ? \
    get_currect_time(DT_CLOCK_SECS, time_txt) :\
    get_currect_time(CLOCK_FORMAT, time_txt);

  int time_font_voffset = settings_get_ClockShowSeconds() == SEC_SHOWING ? 0 : 6; //crunch for vertical alignment
  graphics_draw_text(ctx, time_txt, \
      prv_get_time_font(), \
      GRect (rect.origin.x, rect.origin.y - time_font_voffset, rect.size.w, rect.size.h), \
      GTextOverflowModeWordWrap, \
      GTextAlignmentCenter, \
      NULL);
}

void get_weather(DictionaryIterator *iter, void *context) {

  Tuple *w_error = dict_find(iter, MESSAGE_KEY_WeatherError);
  if (w_error) {
    uint8_t w_err = w_error->value->uint8;
    #if defined (DEBUG)
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Weather Error %d", w_err);
    #endif
    switch (w_err) {
      case 0:
        settings_set_WeatherStatus(WEATHER_OK);
        break;
      case 1:
        settings_set_WeatherStatus(WEATHER_DISABLED);
        break;
      case 2:
        settings_set_WeatherStatus(WEATHER_LOCATION_ERROR);
        break;
      case 3:
        settings_set_WeatherStatus(WEATHER_API_NOT_SET);
        break;
      case 4:
        settings_set_WeatherStatus(WEATHER_API_INVALID);
        break;
      case 5:
        settings_set_WeatherStatus(WEATHER_LOCATION_ID_INVALID);
        break;
      case 6:
        settings_set_WeatherStatus(WEATHER_UNKNOWN_ERROR);
        break;
    } 
  }

  Tuple *w_temp = dict_find(iter, MESSAGE_KEY_WeatherTemperature);
  if (w_temp) {
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

  Tuple *w_time_stamp = dict_find(iter, MESSAGE_KEY_WeatherTimeStamp);
  if (w_time_stamp) {
    weather.WeatherTimeStamp = w_time_stamp->value->uint32;
  }

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
  }

  Tuple *w_rise = dict_find(iter, MESSAGE_KEY_WeatherSunrise);
  if (w_rise) {
    snprintf(weather.WeatherSunrise, sizeof(weather.WeatherSunrise), w_rise->value->cstring);
  }

  Tuple *w_set = dict_find(iter, MESSAGE_KEY_WeatherSunset);
  if (w_set) {
    snprintf(weather.WeatherSunset, sizeof(weather.WeatherSunset), w_set->value->cstring);

  }  
  prv_save_weather();
  layer_mark_dirty(s_this_layer);
}

static void prv_save_weather() {
    persist_write_data(WEATHER_KEY, &weather, sizeof(weather));
}

static void prv_load_weather() {  
  persist_read_data(WEATHER_KEY, &weather, sizeof(weather));
  prv_default_weather_data();
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
  #if defined (DEBUG) 
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Weather send message failed, timeput 5 secs");
  #endif  
  s_timeout_timer = app_timer_register(timeout, prv_timer_timeout_handler, NULL);
}

static void prv_timer_timeout_handler (void *context) {
  bool state = is_force_update;
  if (state) {
    is_force_update = false;
  }
  update_weather(state);
}