#include <pebble.h>
#include "include/bluetooth_m.h"
#include "../settings.h"
#include "../utils/include/timeutils.h"
#include "../utils/include/vibe.h"
#include "../utils/include/ticktimerhelper.h"

//static GBitmap *s_bt_icon;
static GFont statuses_font;
static Layer *this_layer;

static void prv_bt_connection_status(bool state);
static void prv_populate_bt_layer(Layer *, GContext *);
static bool init = false;
static bool bt_connected = false;


void init_bluetooh_layer(GRect rect) {
  this_layer = layer_create(rect);
  layer_set_update_proc(this_layer, prv_populate_bt_layer);
  connection_service_subscribe((ConnectionHandlers) {
    .pebble_app_connection_handler = prv_bt_connection_status,
    .pebblekit_connection_handler = NULL,
  });
  statuses_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_STATUSES_18));
  prv_bt_connection_status(connection_service_peek_pebble_app_connection());
 
}

static void prv_bt_connection_status(bool state) { 
#if defined (DEBUG) 
   APP_LOG(APP_LOG_LEVEL_DEBUG, "BT connection status changed");
#endif 
  bt_connected = state;
  if (!init) {    
#if defined (DEBUG) 
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Not inited");
#endif
    return;
  }
    
  if (state) {
    if (settings_get_VibrateConnected()) {
     do_vibrate(settings_get_VibrateConnectedType());
    } 
  } else {
    if (settings_get_VibrateDisconnected()) {
     do_vibrate(settings_get_VibrateDisconnectedType()); 
    }
  }
  layer_mark_dirty(this_layer);
}

static void prv_get_weather_error_symbol(char *err_symbol) {
  switch (settings_get_WeatherStatus()) {
    case WEATHER_API_INVALID:
    case WEATHER_API_NOT_SET:
      snprintf(err_symbol, 2, "%s", "C\0");
      break;
    case WEATHER_LOCATION_ID_INVALID:    
      snprintf(err_symbol, 2, "%s", "U\0");
      break;
    case WEATHER_UNKNOWN_ERROR:
      snprintf(err_symbol, 2, "%s", "F\0");
      break;
    case WEATHER_LOCATION_ERROR:
      snprintf(err_symbol, 2, "%s", "E\0");
      break;
    default:
      snprintf(err_symbol, 2, "%s", " \0");
      break;
}
}

static void prv_populate_bt_layer(Layer *me, GContext *ctx) {
  #if defined (DEBUG) 
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Draw: STATUS LAYER");
  #endif  
  init = true; //do not vibrate on watchface startup.
  settings_get_theme(ctx);
  graphics_draw_text(ctx, bt_connected ? "B" : "A" , \
    statuses_font, \
    GRect (0, 0, 20, 20), \
    GTextOverflowModeWordWrap, \
    GTextAlignmentCenter, \
    NULL);

  if (settings_get_WeatherStatus() != WEATHER_OK) {
    char weather_err_symbol[2] = {"B"};
    prv_get_weather_error_symbol(weather_err_symbol);
    #if defined (DEBUG)
      APP_LOG(APP_LOG_LEVEL_DEBUG, "weather status %d, BAD status:%s", settings_get_WeatherStatus(), weather_err_symbol);
    #endif
    graphics_draw_text(ctx, weather_err_symbol , \
    statuses_font, \
    GRect (20, 0, 20, 20), \
    GTextOverflowModeWordWrap, \
    GTextAlignmentCenter, \
    NULL);
  }
  
  if (is_quiet_time()) {
    graphics_draw_text(ctx, "D" , \
    statuses_font, \
    GRect (40, 0, 20, 20), \
    GTextOverflowModeWordWrap, \
    GTextAlignmentCenter, \
    NULL);    
  }
  //if (settings_get_ClockFormatSettings() == CF_RESPECT)
  if (strcmp(settings_get_ClockFormat(), "%I:%M") == 0) {    
    graphics_draw_text(ctx, get_Time()->tm_hour < 12 ? "AM" : "PM" , \
    fonts_get_system_font(FONT_KEY_GOTHIC_18), \
    GRect (60, 0, 20, 20), \
    GTextOverflowModeWordWrap, \
    GTextAlignmentCenter, \
    NULL);    
  }
}

void deinit_bluetooth_layer() {
  connection_service_unsubscribe();
  fonts_unload_custom_font(statuses_font);
  if (this_layer) {
    layer_destroy(this_layer);
  }
}

Layer* get_layer_bluetooth() {
  return this_layer;
}


bool is_bt_connected() {
  return bt_connected;
}