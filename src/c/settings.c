#include <pebble.h>
#include "settings.h"



typedef struct ClaySettings {
//  char WeatherAPIKey[33];
  PERIOD WeatherUpdatePeriod;
  char ClockFormat[8];
  uint8_t RespectQuietTime;
  uint8_t VibrateDuringCharged;
  uint8_t VibrateConnected;
  VIBE VibrateConnectedType;
  uint8_t VibrateDisconnected;
  VIBE VibrateDisconnectedType;
  uint8_t VibrateDuringCharging;
  uint8_t VibratePeriodic;
  PERIOD VibratePeriodicPeroid;
  VIBE VibratePeriodicType;
  uint8_t SundayFirst;

  // bool showPebbleConnection;
  // bool showPebbleBattery;
  // bool showPebbleBatteryPercents;
} __attribute__((__packed__)) ClaySettings;

static ClaySettings settings;
static callback_ptr settings_update_handler = NULL;

static void prv_default_settings() {
  strcpy(settings.ClockFormat, "%H:%M\0");
  //strcpy(settings.WeatherAPIKey, "not_set");
  settings.WeatherUpdatePeriod = P_1H;
  settings.RespectQuietTime = 1;
  settings.VibrateDuringCharged = 0;
  settings.VibrateConnected = 1;
  settings.VibrateConnectedType = VP_SHORT;
  settings.VibrateDisconnected = 1;
  settings.VibrateDisconnectedType = VP_DOUBLE_SHORT;
  settings.VibrateDuringCharging = 0;
  settings.VibratePeriodic = 1;
  settings.VibratePeriodicPeroid = P_1H;
  settings.VibratePeriodicType = VP_LONG;
  settings.SundayFirst = 0;
  // settings.showPebbleBattery = true;
  // settings.showPebbleConnection = true;
  // settings.showPebbleBatteryPercents = true;
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

// char* settings_get_weather_apikey() {
//   return settings.WeatherAPIKey;
// }

static bool get_bool (uint8_t settings_bool) {
    return settings_bool == 1;
}

bool settings_get_RespectQuietTime() {
  return get_bool(settings.RespectQuietTime);
}

bool settings_get_VibrateDuringCharging() {
  return get_bool(settings.VibrateDuringCharging);
}

bool settings_get_VibrateConnected() {
  return get_bool(settings.VibrateConnected);
}

static VIBE get_vibe(char *settings_vibe) {
  if (strcmp(settings_vibe, "1") == 0) {
      return VP_SHORT;
  }
  if (strcmp(settings_vibe, "2") == 0) {
      return VP_DOUBLE_SHORT;
  }
  if (strcmp(settings_vibe, "3") == 0) {
      return VP_TRIPLE_SHORT;
  }
  return VP_LONG;
}

VIBE settings_get_VibrateConnectedType() {
  return settings.VibrateConnectedType;
}

bool settings_get_VibrateDisconnected() {
  return get_bool(settings.VibrateDisconnected);
}

VIBE settings_get_VibrateDisconnectedType() {
  return settings.VibrateDisconnectedType;
}

bool settings_get_SundayFirst() {
  return get_bool(settings.SundayFirst);
}

// bool settings_get_VibrateDuringCharging() {
//   return get_bool(settings.VibrateDuringCharging);
// };

bool settings_get_VibratePeriodic() {
  return get_bool(settings.VibratePeriodic);
}

static PERIOD get_period (char *settings_per) {
  if (strcmp(settings_per, "15") == 0) {
      return P_15MIN;
  }

  if (strcmp(settings_per, "30") == 0) {
      return P_30MIN;
  }

  if (strcmp(settings_per, "60") == 0) {
      return P_1H;
  }

  if (strcmp(settings_per, "3h") == 0) {
      return P_3H;
  }
  return P_6H;
}

PERIOD settings_get_VibratePeriodicPeroid() {
  return settings.VibratePeriodicPeroid;
}

PERIOD settings_get_WeatherUpdatePeriod() {
  return settings.WeatherUpdatePeriod;
}

VIBE settings_get_VibratePeriodicType() {
  return settings.VibratePeriodicType;
}

static void prv_load_settings() {
  // Load the default settings
  prv_default_settings();
//  APP_LOG(APP_LOG_LEVEL_DEBUG, "Loading persist settings");
  // Read settings from persistent storage, if they exist
  persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));

//  APP_LOG(APP_LOG_LEVEL_DEBUG, "Loaded API Key is [%s]", settings.WeatherAPIKey);
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

  // Tuple *peb_conn_t = dict_find(iter, MESSAGE_KEY_PebbleConnection);
  // if (peb_conn_t) {
  //   settings.showPebbleConnection = peb_conn_t->value->int32 == 1;
  // }
  //
  // Tuple *peb_battery_t = dict_find(iter, MESSAGE_KEY_PebbleBatteryStatus);
  // if (peb_battery_t) {
  //   settings.showPebbleBattery = peb_battery_t->value->int32 == 1;
  // }
  //
  // Tuple *peb_battery_pc_t = dict_find(iter, MESSAGE_KEY_PebbleBatteryPercents);
  // if (peb_battery_pc_t) {
  //   settings.showPebbleBatteryPercents = peb_battery_pc_t->value->int32 == 1;
  // }
  //
  //
  // APP_LOG(APP_LOG_LEVEL_DEBUG, "opt are : %i and %i", settings.showPebbleConnection, settings.showPebbleBattery);


  // Tuple *weather_apikey_t = dict_find(iter, MESSAGE_KEY_WeatherAPIKey);
  // if (weather_apikey_t) {
  //   strcpy(settings.WeatherAPIKey, weather_apikey_t->value->cstring);
  //   settings_update_handler(UF_WEATHER);
  // }

  Tuple *vibr_conn = dict_find(iter, MESSAGE_KEY_VibrateConnected);
  if (vibr_conn) {
      settings.VibrateConnected = vibr_conn->value->uint8;
  }

  Tuple *vibr_conn_type = dict_find(iter, MESSAGE_KEY_VibrateConnectedType);
  if (vibr_conn_type) {
    settings.VibrateConnectedType = get_vibe(vibr_conn_type->value->cstring);
  }

  Tuple *vibr_disconn = dict_find(iter, MESSAGE_KEY_VibrateDisconnected);
  if (vibr_disconn) {
      settings.VibrateDisconnected = vibr_disconn->value->uint8;
  }

  Tuple *vibr_disconn_type = dict_find(iter, MESSAGE_KEY_VibrateDisconnectedType);
  if (vibr_disconn_type) {
    settings.VibrateDisconnectedType = get_vibe(vibr_disconn_type->value->cstring);
  }

  Tuple *vibr_chard = dict_find(iter, MESSAGE_KEY_VibrateDuringCharging);
  if (vibr_chard) {
      settings.VibrateDuringCharging = vibr_chard->value->uint8;
  }

  Tuple *vibr_per = dict_find(iter, MESSAGE_KEY_VibratePeriodic);
  if (vibr_per) {
      settings.VibratePeriodic = vibr_per->value->uint8;
  }

  Tuple *vibr_per_per = dict_find(iter, MESSAGE_KEY_VibratePeriodicPeroid);
  if (vibr_per_per) {
    settings.VibratePeriodicPeroid = get_period(vibr_per_per->value->cstring);
  }

  Tuple *w_per = dict_find(iter, MESSAGE_KEY_WeatherUpdatePeriod);
  if (w_per) {
    settings.WeatherUpdatePeriod = get_period(w_per->value->cstring);
  }

  Tuple *vibr_per_type = dict_find(iter, MESSAGE_KEY_VibratePeriodicType);
  if (vibr_per_type) {
    settings.VibratePeriodicType = get_vibe(vibr_per_type->value->cstring);
  }

  Tuple *sunday_first = dict_find(iter, MESSAGE_KEY_SundayFirst);
  if (vibr_per_type) {
    //char sf_option[4] = sunday_first->value->cstring;
    settings.SundayFirst = strcmp(sunday_first->value->cstring, "sun") == 0 ? 1 : 0;
  }
  save_settings();
}

void init_settings(callback_ptr callback) {
  settings_update_handler = callback;
  prv_load_settings();
}
