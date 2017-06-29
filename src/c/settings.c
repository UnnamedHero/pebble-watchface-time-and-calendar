#include <pebble.h>
#include "settings.h"



typedef struct ClaySettings {
  char WeatherAPIKey[33];
  char ClockFormat[6];
} __attribute__((__packed__)) ClaySettings;

static ClaySettings settings;
static callback_ptr settings_update_handler = NULL;

static void prv_default_settings() {
  strcpy(settings.ClockFormat, "%H:%M");
  strcpy(settings.WeatherAPIKey, "not_set");
}

char* settings_get_clockformat() {
  //strcmp(settings.ClockFormat, "cf_respect" == 0 ?
  // char *result = "%I:%M";
  // switch (settings.ClockFormat) {
  //   case 0:
  //     break;
  //   case 1:
  //     result = "%H:%M";
  //     break;
  //   case 2:
  //     result = "%I:%M";
  //     break;
  // }
  return settings.ClockFormat;
}

char* settings_get_weather_apikey() {
  return settings.WeatherAPIKey;
}

static void prv_load_settings() {
  // Load the default settings
  prv_default_settings();
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Loading persist settings");
  // Read settings from persistent storage, if they exist
  persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Loaded API Key is [%s]", settings.WeatherAPIKey);
}

void save_settings() {
    persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
    settings_update_handler(UF_NOTHING);
}

void populate_settings(DictionaryIterator *iter, void *context) {
  Tuple *clock_format_t = dict_find(iter, MESSAGE_KEY_ClockFormat);
  if (clock_format_t) {
    if (strcmp(clock_format_t->value->cstring, "cf_respect") == 0) {
      clock_is_24h_style() ? strcpy(settings.ClockFormat, "%H:%M") : \
                             strcpy(settings.ClockFormat, "%I:%M");
    } else {
      strcpy(settings.ClockFormat, clock_format_t->value->cstring);
    }
  }

  Tuple *weather_apikey_t = dict_find(iter, MESSAGE_KEY_WeatherAPIKey);
  if (weather_apikey_t) {
    strcpy(settings.WeatherAPIKey, weather_apikey_t->value->cstring);
    settings_update_handler(UF_WEATHER);
  }

  save_settings();
}

void init_settings(callback_ptr callback) {
  settings_update_handler = callback;
  prv_load_settings();
}
