#include <pebble.h>
#include "settings.h"



typedef struct ClaySettings {
  char ClockFormat[6];
} __attribute__((__packed__)) ClaySettings;

static ClaySettings settings;
static callback_ptr settings_update_handler = NULL;

static void prv_default_settings() {
  strcpy(settings.ClockFormat, "%H:%M");
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

static void prv_load_settings() {
  // Load the default settings
  prv_default_settings();
  // Read settings from persistent storage, if they exist
  persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
}

void save_settings() {
    persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
    settings_update_handler();
}

static void prv_inbox_received_handler(DictionaryIterator *iter, void *context) {
  Tuple *clock_format_t = dict_find(iter, MESSAGE_KEY_ClockFormat);
  if (clock_format_t) {
    if (strcmp(clock_format_t->value->cstring, "cf_respect") == 0) {
      clock_is_24h_style() ? strcpy(settings.ClockFormat, "%H:%M") : \
                             strcpy(settings.ClockFormat, "%I:%M");
    } else {
      strcpy(settings.ClockFormat, clock_format_t->value->cstring);
    }

    APP_LOG(APP_LOG_LEVEL_DEBUG, "from config:[%s]", settings.ClockFormat);
  }

  save_settings();
}

void init_settings(callback_ptr callback) {
  settings_update_handler = callback;
  app_message_register_inbox_received(prv_inbox_received_handler);
  app_message_open(128, 128);
  prv_load_settings();
}
