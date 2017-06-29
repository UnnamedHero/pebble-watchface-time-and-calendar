#include <pebble.h>
#include "weather.h"

void init_weather(char *apiKey) {
  DictionaryIterator *out_iter;
  APP_LOG(APP_LOG_LEVEL_DEBUG, "sending apikey: %s", apiKey);
  AppMessageResult result = app_message_outbox_begin(&out_iter);
  if (result == APP_MSG_OK) {
    dict_write_cstring(out_iter, MESSAGE_KEY_WeatherAPIKey, apiKey);

    result = app_message_outbox_send();
    if (result != APP_MSG_OK) {
      APP_LOG(APP_LOG_LEVEL_ERROR, "Error sending the outbox: %s", apiKey);
    }
  } else {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Error preparing_outbox: %d", (int)result);
  }
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
