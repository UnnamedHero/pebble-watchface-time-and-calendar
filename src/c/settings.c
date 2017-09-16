
#include "settings.h"
#include "utils/include/timeutils.h"

static const uint32_t storage_version_key = 777;
//static const int current_storage_version = 2;

typedef struct ClaySettings {
  API_STATUS WeatherAPIKeyStatus;
  PERIOD WeatherUpdatePeriod;
  char ClockFormat[8];
  char DateFormat[12];
  uint8_t RespectQuietTime;
#if defined (PBL_PLATFORM_APLITE)
  uint8_t QT;
  uint8_t QTHourBegin;
  uint8_t QTHourEnd;
  uint8_t QTMinBegin;
  uint8_t QTMinEnd;
#endif
  uint8_t ForecastEnabled;
  int FontColorHex;
  int BackgroundColorHex;
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
  CAL_WEEK_VIEW CalendarWeeks;
  uint8_t CalendarBoldWeekDay;
  uint8_t CalendarInvertWeekDay;
  uint8_t CalendarBoldToday;
  uint8_t CalendarInvertToday;
  uint8_t CalendarSmallOtherDays;
  uint8_t SwitchBackTimeout;
  // bool showPebbleConnection;
  // bool showPebbleBattery;
  // bool showPebbleBatteryPercents;
} __attribute__((__packed__)) ClaySettings;

static ClaySettings settings;
static callback_ptr settings_update_handler = NULL;

static void prv_post_load_settings();

static void prv_default_settings() {
  
  //strcpy(settings.ClockFormat, "%H:%M\0");
  //strcpy(settings.DateFormat, "%Y.%m.%d\0");
  //snprintf(settings.ClockFormat, sizeof(settings.ClockFormat), "%H:%M");
  //snprintf(settings.DateFormat, sizeof(settings.DateFormat), "%Y.%m.%d");
  
  settings.WeatherUpdatePeriod = P_1H;
  settings.FontColorHex = 0xffffff;
  settings.BackgroundColorHex = 0x000000;  
  settings.RespectQuietTime = 0;
#if defined (PBL_PLATFORM_APLITE)
  settings.QT = 0;
  settings.QTHourBegin = 23;
  settings.QTHourEnd = 6;
  settings.QTMinBegin = 0;
  settings.QTMinEnd = 0;
#endif
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
  settings.CalendarWeeks = CAL_WV_PCN;
  settings.CalendarBoldWeekDay = 1;
  settings.CalendarInvertWeekDay = 0;
  settings.CalendarBoldToday = 0;
  settings.CalendarInvertToday = 1;
  settings.SwitchBackTimeout = 15;
  // settings.showPebbleBattery = true;
  // settings.showPebbleConnection = true;
  // settings.showPebbleBatteryPercents = true;
}

static void increase_current_storage_version(int current) {
  persist_write_int(storage_version_key, current);
  prv_post_load_settings();
}

static void prv_post_load_settings() {  

  helper_str_filler(settings.DateFormat, "%Y.%m.%d\0");
  helper_str_filler(settings.ClockFormat, "%H:%M\0");

  bool has_storage_version = persist_exists(storage_version_key);

  const int last_storage_version = has_storage_version ? \
          persist_read_int(storage_version_key) :\
          0;
  if (settings.FontColorHex == settings.BackgroundColorHex) {
      settings.FontColorHex = 0xffffff;
      settings.BackgroundColorHex = 0x000000;
  }

  switch (last_storage_version) {
    case 0:
      settings.FontColorHex = 0xffffff;
      settings.BackgroundColorHex = 0x000000;
      settings.SwitchBackTimeout = 15;
      increase_current_storage_version(last_storage_version + 1);
      break;        
    case 2:    
      //helper_str_filler(settings.WeatherAPIKey, "not_set");      
      //increase_current_storage_version(last_storage_version + 1);
      break;
    case 3:
      break;

    default:
      break;
  }
}


char* settings_get_clockformat() {
  return settings.ClockFormat;
}

// char* settings_get_WeatherAPIKey() {
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

static CAL_WEEK_VIEW get_weekview(char *settings_weekview) {
  if (strcmp(settings_weekview, "cal_ppc") == 0) {
      return CAL_WV_PPC;
  }
  if (strcmp(settings_weekview, "cal_pcn") == 0) {
      return CAL_WV_PCN;
  }
  if (strcmp(settings_weekview, "cal_cnn") == 0) {
      return CAL_WV_CNN;
  }
  return CAL_WV_PCN;
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

int settings_get_BackgroundColorHex() {
  return settings.BackgroundColorHex;
}

int settings_get_FontColorHex() {
  return settings.FontColorHex;
}

bool settings_get_VibratePeriodic() {
  return get_bool(settings.VibratePeriodic);
}

bool settings_get_ForecastEnabled() {
  return get_bool(settings.ForecastEnabled);
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

CAL_WEEK_VIEW settings_get_CalendarWeekView() {
  return settings.CalendarWeeks;
}

#if defined (PBL_PLATFORM_APLITE)
uint8_t settings_get_QTHourBegin() {
  return settings.QTHourBegin;
}

uint8_t settings_get_QTHourEnd() {
  return settings.QTHourEnd;
}

uint8_t settings_get_QTMinBegin() {
  return settings.QTMinBegin;
}

uint8_t settings_get_QTMinEnd() {
  return settings.QTMinEnd;
}
#endif

bool settings_get_CalendarBoldWeekDay() {
  return get_bool(settings.CalendarBoldWeekDay);
}

bool settings_get_CalendarInvertWeekDay() {
  return get_bool(settings.CalendarInvertWeekDay);
}

bool settings_get_CalendarBoldToday() {
  return get_bool(settings.CalendarBoldToday);
}

bool settings_get_CalendarSmallOtherDays() {
  return get_bool(settings.CalendarSmallOtherDays);
}

bool settings_get_CalendarInvertToday() {
  return get_bool(settings.CalendarInvertToday);
}

char *settings_get_DateFormat() {
  return settings.DateFormat;
}

uint8_t settings_get_SwitchBackTimeout() {
  return settings.SwitchBackTimeout;
}

API_STATUS settings_get_WeatherAPIKeyStatus() {
  return settings.WeatherAPIKeyStatus;
}

void helper_str_filler(char *item, char* filler) {
  if (strlen(item) == 0) {
    strcpy(item, filler);
  }
}

static void prv_load_settings() {
  // Load the default settings
  prv_default_settings();

  // Read settings from persistent storage, if they exist
  persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
  prv_post_load_settings();
}

void save_settings() {
    persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
    //persist_write_int(storage_version_key, current_storage_version);
    settings_update_handler();
}

void settings_get_theme(GContext *ctx) {
    graphics_context_set_fill_color(ctx, GColorFromHEX(settings.BackgroundColorHex));
    graphics_context_set_text_color(ctx, GColorFromHEX(settings.FontColorHex));  
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

  Tuple *w_api = dict_find(iter, MESSAGE_KEY_WeatherAPIKey);
  if (w_api) {
    char *apikey = w_api->value->cstring;
    if (strcmp(apikey, "not_set") == 0) {
      settings.WeatherAPIKeyStatus = API_NOT_SET;
    } else if (strcmp(apikey, "invalid_api_key") == 0) {
      settings.WeatherAPIKeyStatus = API_INVALID;
    } else {
      settings.WeatherAPIKeyStatus = API_OK;
    }    
  }

  Tuple *date_fmt = dict_find(iter, MESSAGE_KEY_DateFormat);
  if (date_fmt) {
    strcpy(settings.DateFormat, date_fmt->value->cstring);    
  }

   Tuple *font_c = dict_find(iter, MESSAGE_KEY_FontColor);
   if (font_c) {
      settings.FontColorHex = font_c->value->int32;
   }

   Tuple *bkg_c = dict_find(iter, MESSAGE_KEY_BackgroundColor);
   if (bkg_c) {
      settings.BackgroundColorHex = bkg_c->value->int32;
   }   

#if defined (PBL_PLATFORM_APLITE)
  Tuple *qt = dict_find(iter, MESSAGE_KEY_QuietTime);
  if (qt) {
      settings.QT = qt->value->uint8;
  }

   Tuple *qthb = dict_find(iter, MESSAGE_KEY_QuietTimeBegin);
   if (qthb) {
      settings.QTHourBegin = qthb->value->uint8;
   }

   Tuple *qtmb = dict_find(iter, MESSAGE_KEY_QuietTimeBegin + 1);
   if (qtmb) {
      settings.QTMinBegin = qtmb->value->uint8;
   }

   Tuple *qthe = dict_find(iter, MESSAGE_KEY_QuietTimeEnd);
   if (qthe) {
      settings.QTHourEnd = qthe->value->uint8;
   }
   
   Tuple *qtme = dict_find(iter, MESSAGE_KEY_QuietTimeEnd + 1);
   if (qtme) {
      settings.QTMinEnd = qtme->value->uint8;
   }


  // Tuple *qtb = dict_find(iter, MESSAGE_KEY_QuietTimeBegin);
  // if (qtb) {
  //     settings.QTHourBegin = get_hour_from_str(qtb->value->cstring);
  //     settings.QTMinBegin = get_mins_from_str(qtb->value->cstring);
  // }
  //
  // Tuple *qte = dict_find(iter, MESSAGE_KEY_QuietTimeEnd);
  // if (qte) {
  //     settings.QTHourEnd = get_hour_from_str(qte->value->cstring);
  //     settings.QTMinEnd = get_mins_from_str(qte->value->cstring);
  // }
#endif

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

  Tuple *f_on = dict_find(iter, MESSAGE_KEY_ForecastType);
  if (f_on) {    
    settings.ForecastEnabled = strcmp(f_on->value->cstring, "ft_off") == 0 ? 0 : 1;
  }

  Tuple *cwv = dict_find(iter, MESSAGE_KEY_CalendarWeeks);
  if (cwv) {
     settings.CalendarWeeks = get_weekview(cwv->value->cstring);
  }

  Tuple *cal_boldwd = dict_find(iter, MESSAGE_KEY_CalendarWeekDayMark);
  if (cal_boldwd) {
     settings.CalendarBoldWeekDay = cal_boldwd->value->uint8;
  }

  Tuple *cal_invwd = dict_find(iter, MESSAGE_KEY_CalendarWeekDayMark + 1);
  if (cal_invwd) {
     settings.CalendarInvertWeekDay = cal_invwd->value->uint8;
  }

  Tuple *cal_boldtoday = dict_find(iter, MESSAGE_KEY_CalendarDayMark);
  if (cal_boldtoday) {
     settings.CalendarBoldToday = cal_boldtoday->value->uint8;
  }

  Tuple *cal_invtoday = dict_find(iter, MESSAGE_KEY_CalendarDayMark + 1);
  if (cal_invtoday) {
     settings.CalendarInvertToday = cal_invtoday->value->uint8;
  }

  Tuple *cal_smdays = dict_find(iter, MESSAGE_KEY_CalendarSmallOtherDays);
  if (cal_smdays) {
     settings.CalendarSmallOtherDays = cal_smdays->value->uint8;
  }

  Tuple *swtime = dict_find(iter, MESSAGE_KEY_SwitchBackTimeout);
  if (swtime) {
     settings.SwitchBackTimeout = swtime->value->uint8;
  }


  save_settings();
//  settings_update_handler();
}

void init_settings(callback_ptr callback) {
  settings_update_handler = callback;
  prv_load_settings();  
}
