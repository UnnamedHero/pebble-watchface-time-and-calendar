#include <pebble.h>
#include "include/messagesystem.h"
#include "../settings.h"
#include "../windows/include/time-window.h"
#include "../windows/include/forecast-window.h"
#include "../modules/include/bluetooth_m.h"


static bool s_js_ready;
static void prv_inbox_received_handler(DictionaryIterator *, void *);
static bool busy = true;

void init_message_system() {
  app_message_register_inbox_received(prv_inbox_received_handler);
  app_message_open(512, 128);

}


static void prv_inbox_received_handler(DictionaryIterator *iter, void *context) {
  #if defined (DEBUG) 
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Inbox message received");
  #endif
  Tuple *js_ready_t = dict_find(iter, MESSAGE_KEY_JSReady);
  if (js_ready_t) {
    #if defined (DEBUG) 
      APP_LOG(APP_LOG_LEVEL_DEBUG, "JS Ready received");
    #endif
    s_js_ready = true;
    busy = false;
    ready_for_weather(false);
    if (settings_get_ForecastEnabled()) {
      ready_for_forecast(false);
    }
    //init_weather(settings_get_weather_apikey());
  }

  Tuple *config_marker = dict_find(iter, MESSAGE_KEY_ConfigMarker);
  if (config_marker) {
    #if defined (DEBUG) 
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Config received");
    #endif
    populate_settings(iter, context);
    if (settings_get_WeatherStatus() == WEATHER_OK) {
      ready_for_weather(true);
      if (settings_get_ForecastEnabled()) {
        ready_for_forecast(true);
      }
    }
  }

  Tuple *w_error = dict_find(iter, MESSAGE_KEY_WeatherError);
  if (w_error) {
    #if defined (DEBUG)
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Weather Error received.");      
    #endif
    populate_settings(iter, context);
    if (w_error->value->uint8 > 2) { 
      return;
    }
  }

  Tuple *weather_marker = dict_find(iter, MESSAGE_KEY_WeatherMarker);
  if (weather_marker) 
    {
      simple_weather_update(iter, context);
    }
  
  Tuple *weather_forecast = dict_find(iter, MESSAGE_KEY_WeatherMarkerForecast);
  if (weather_forecast) {
      forecast_update_disp(iter, context);
    }    
}


void send_message(Tuplet *data, int data_array_size, message_failed_callback_ptr message_failed_callback) {
  if (busy) {
    #if defined (DEBUG) 
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Message system busy");
    #endif    
    message_failed_callback();
  }
  busy = true;
  if (!s_js_ready) {
    #if defined (DEBUG) 
      APP_LOG(APP_LOG_LEVEL_DEBUG, "JS not ready yet");
    #endif
    busy = false;
    message_failed_callback();
  }
  
  if (!is_bt_connected()) {
    #if defined (DEBUG) 
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Bluetooth not connected, aborting sending data");
    #endif
    busy = false;    
    return;
  }

  DictionaryIterator *iter;
  AppMessageResult prepare_result = app_message_outbox_begin(&iter);
  if (prepare_result != APP_MSG_OK) {
    message_failed_callback();
  }

  for (int i = 0; i < data_array_size; i++ ) {
    const Tuplet item = data[i];
    DictionaryResult dict_write_result = dict_write_tuplet(iter, &item);
    if (dict_write_result != DICT_OK) {
      APP_LOG(APP_LOG_LEVEL_ERROR, "Error writing tuplet to outbox, err code is %d", dict_write_result);
      return;
    }
  }
  
  AppMessageResult send_result = app_message_outbox_send();
  if (send_result != APP_MSG_OK) {
    message_failed_callback();
    busy = false;
  }
  #if defined (DEBUG) 
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Data sended!");
  #endif
  
  busy = false;
}


void deinit_message_system() {
  //app_sync_deinit(&sync);
}