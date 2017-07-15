#include <pebble.h>
#include "include/weather_m.h"

static Layer *s_this_layer;
static void prv_populate_weather_layer(Layer *, GContext *);
//static GBitmap *s_arrow_bitmap;
static char* wind_directions[] = {"N", "NNE", "NE", "ENE", "E", "ESE", "SE", "SSE", "S", "SSW", "SW", "WSW", "W", "WNW", "NW", "NNW"};
typedef struct WeatherData {
  uint8_t WeatherReady;
  int WeatherTemperature;
  char WeatherDesc[32];
  uint8_t WeatherHumidity;
  int WeatherPressure;
  uint8_t WeatherWindSpeed;
  int WeatherWindDirection;
} __attribute__((__packed__)) WeatherData;

WeatherData weather;

void prv_default_weather_data() {
  weather.WeatherReady = 0;
}

int prv_get_wind_direction(int azim) {
  int normalized = azim + 11;
  int azimuth = normalized > 360 ? normalized - 360 : normalized;
  return (int)(azimuth / 22.5);
  //int big_azumuth = azumith * 10;
  //int init = (azimuth + 110) > 3600 ? 3600 - 110 : azimuth - 11;
//    float m = round(3.7);
//    return init / 16;360
}

void init_weather_layer(GRect bounds) {
  s_this_layer = layer_create(bounds);
  layer_set_update_proc(s_this_layer, prv_populate_weather_layer);
  // s_arrow_bitmap = gbitmap_create_with_resource(RESOURCE_ID_ARROW);
  // DictionaryIterator *out_iter;
  // APP_LOG(APP_LOG_LEVEL_DEBUG, "sending apikey: %s", apiKey);
  // AppMessageResult result = app_message_outbox_begin(&out_iter);
  // if (result == APP_MSG_OK) {
  //   dict_write_cstring(out_iter, MESSAGE_KEY_WeatherAPIKey, apiKey);
  //
  //   result = app_message_outbox_send();
  //   if (result != APP_MSG_OK) {
  //     APP_LOG(APP_LOG_LEVEL_ERROR, "Error sending the outbox: %s", apiKey);
  //   }
  // } else {
  //   APP_LOG(APP_LOG_LEVEL_ERROR, "Error preparing_outbox: %d", (int)result);
  // }
}

void deinit_weather_layer() {
  if (s_this_layer) {
    layer_destroy(s_this_layer);
  }
  // if (s_arrow_bitmap) {
  //   gbitmap_destroy(s_arrow_bitmap);
  // }
}

Layer* get_layer_weather() {
  return s_this_layer;
}

void update_weather() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Let's get weather!");
  // Begin dictionary
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

  // Add a key-value pair
  dict_write_uint8(iter, MESSAGE_KEY_WeatherMarker, 0);

  // Send the message!
  app_message_outbox_send();
}

void prv_populate_weather_layer(Layer *me, GContext *ctx) {
  if (weather.WeatherReady == 1) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Draw: WEATHER");
    GRect this_bounds = layer_get_bounds(s_this_layer);
    //int wind_direction = prv_get_wind_direction()
    static char weather_text[64];
//    APP_LOG(APP_LOG_LEVEL_DEBUG, "az is %d, wind is %d", weather.WeatherWindDirection, prv_get_wind_direction(weather.WeatherWindDirection));
    snprintf(weather_text, sizeof(weather_text), "%d°, %s, hm: %d%%, %s, %d m/s, %d mmHg", \
        weather.WeatherTemperature,\
        weather.WeatherDesc,\
        weather.WeatherHumidity,\
        wind_directions[prv_get_wind_direction(weather.WeatherWindDirection)],\
        weather.WeatherWindSpeed, \
        (int)(weather.WeatherPressure * 0.75) \
      );
    graphics_draw_text(ctx, weather_text, \
        fonts_get_system_font(FONT_KEY_GOTHIC_14), \
        this_bounds, \
        GTextOverflowModeWordWrap, \
        GTextAlignmentLeft, \
        NULL);
    // GPoint wind_p = GPoint(140, 10);
  //  graphics_draw_bitmap_in_rect(ctx, s_arrow_bitmap, GRect (2, 18, 10, 10));
  }


}

void get_weather(DictionaryIterator *iter, void *context) {
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

  Tuple *w_humid = dict_find(iter, MESSAGE_KEY_WeatherHumidity);
  if (w_humid) {
    weather.WeatherHumidity = w_humid->value->uint8;
  }

  Tuple *w_press = dict_find(iter, MESSAGE_KEY_WeatherPressure);
  if (w_press) {
    weather.WeatherPressure = w_press->value->int32;
  }

  Tuple *w_ws = dict_find(iter, MESSAGE_KEY_WeatherWindSpeed);
  if (w_ws) {
    weather.WeatherWindSpeed = w_ws->value->uint8;
  }

  Tuple *w_wdir = dict_find(iter, MESSAGE_KEY_WeatherWindDirection);
  if (w_wdir) {
    weather.WeatherWindDirection = w_wdir->value->int32;
  }
  layer_mark_dirty(s_this_layer);
}
