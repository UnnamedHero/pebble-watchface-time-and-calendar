#pragma once

#include <pebble.h>
#define SETTINGS_KEY 1

typedef enum Update_Flag {
  UF_NOTHING,
  UF_WEATHER
} UPDATE_FLAG;

typedef enum Vibrate_Pattern {
  VP_SHORT,
  VP_DOUBLE_SHORT,
  VP_TRIPLE_SHORT,
  VP_LONG
} VIBE;

typedef enum Periods {
  P_15MIN,
  P_30MIN,
  P_1H,
  P_3H,
  P_6H,
} PERIOD;

typedef enum DateTimeFormat {
  YYYY_MM_DD,
  CLOCK_FORMAT
} DT_FORMAT;

typedef enum CalendarWeekView {
  CAL_WV_PPC,
  CAL_WV_PCN,
  CAL_WV_CNN
} CAL_WEEK_VIEW;

typedef enum WeatherRequest {
  W_DAY,
  W_FORECAST,
  W_BOTH
} W_REQ;


typedef void (*callback_ptr)();

void init_settings(callback_ptr callback);
void populate_settings(DictionaryIterator *iter, void *context);
void save_settings();


char* settings_get_clockformat();
//char* settings_get_weather_apikey();
PERIOD settings_get_WeatherUpdatePeriod();
bool settings_get_RespectQuietTime();
bool settings_get_VibrateDuringCharging();
bool settings_get_VibrateConnected();
VIBE settings_get_VibrateConnectedType();
bool settings_get_VibrateDisconnected();
VIBE settings_get_VibrateDisconnectedType();
bool settings_get_VibrateDuringCharging();
bool settings_get_VibratePeriodic();
PERIOD settings_get_VibratePeriodicPeroid();
VIBE settings_get_VibratePeriodicType();
bool settings_get_SundayFirst();
CAL_WEEK_VIEW settings_get_CalendarWeekView();
bool settings_get_CalendarBoldWeekDay();
bool settings_get_CalendarInvertWeekDay();
bool settings_get_CalendarBoldToday();
bool settings_get_CalendarInvertToday();
char* settings_get_DateFormat();
int settings_get_BackgroundColorHex();
int settings_get_FontColorHex();
void settings_get_theme(GContext *);

#if defined (PBL_PLATFORM_APLITE)
uint8_t settings_get_QTHourBegin();
uint8_t settings_get_QTHourEnd();
uint8_t settings_get_QTMinBegin();
uint8_t settings_get_QTMinEnd();
#endif