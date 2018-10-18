#pragma once

#include <pebble.h>
#define SETTINGS_KEY 1
#define SETTINGS_SECONDS 2

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
  CLOCK_FORMAT,
  DT_CLOCK_SECS
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

typedef enum APYKeyStatus {
  API_NOT_SET,  
  API_INVALID,
  API_OK
} API_STATUS;

typedef enum WeatherStatus {
  WEATHER_OK,
  WEATHER_DISABLED,
  WEATHER_LOCATION_ERROR,
  WEATHER_API_NOT_SET,
  WEATHER_API_INVALID,
  WEATHER_LOCATION_ID_INVALID,
  WEATHER_UNKNOWN_ERROR,
  WEATHER_API_BANNED
} WEATHER_STATUS;

typedef enum ClockFormatSettings {
  CF_RESPECT_PEBBLE,
  CF_24H,
  CF_12H  
} CLOCK_FORMAT_SETTINGS;

typedef enum TimeFont {
  TF_BEBAS,
  TF_DIGITAL
} TIME_FONT;

typedef enum ClockShowSeconds {
  SEC_DISABLED,
  SEC_ENABLED,
  SEC_SHOWING
} CLOCK_SECONDS;

typedef enum PebbleShakeAction {
  PSA_NOTHING,
  PSA_FORECAST,
  PSA_SECONDS,
} PEBBLE_SHAKE_ACTION;

typedef enum PebbleHealthMetric {
  PHM_STEPS,
  PHM_DISTANCE_M,
  PHM_DISTANCE_FEET,
  PHM_CALORIES,
} PEBBLE_HEALTH_METRIC;

typedef void (*callback_ptr)();

void init_settings(callback_ptr callback);
void populate_settings(DictionaryIterator *iter, void *context);
void save_settings();

void helper_str_filler(char *item, char* filler);

//char* settings_get_clockformat();

//char* settings_get_WeatherAPIKey();
//API_STATUS settings_get_WeatherAPIKeyStatus();
PERIOD settings_get_WeatherUpdatePeriod();
bool settings_get_VibrateDuringQuietTime();
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
bool settings_get_CalendarSmallOtherDays();
char* settings_get_DateFormat();
char* settings_get_ClockFormat();
int settings_get_BackgroundColorHex();
int settings_get_FontColorHex();
void settings_get_theme(GContext *);
uint8_t settings_get_SwitchBackTimeout();
bool settings_get_ForecastEnabled();
CLOCK_FORMAT_SETTINGS settings_get_ClockFormatSettings();
WEATHER_STATUS settings_get_WeatherStatus();
void settings_set_WeatherStatus(WEATHER_STATUS);
TIME_FONT settings_get_TimeFont();
CLOCK_SECONDS settings_get_ClockShowSeconds();
PEBBLE_SHAKE_ACTION settings_get_PebbleShakeAction();
uint8_t settings_get_SwitchBackTimeoutSeconds();
void settings_set_ClockShowSeconds_showing();
void settings_set_ClockShowSeconds_enabled();
void settings_set_ClockShowSeconds_disabled();
bool settings_get_ShakeTwice();
bool settings_get_ColorTimeShift();
uint8_t settings_get_ColorShiftHourBegin();
uint8_t settings_get_ColorShiftMinuteBegin();
uint8_t settings_get_ColorShiftHourEnd();
uint8_t settings_get_ColorShiftMinuteEnd();
int settings_get_ShiftBackgroundColor();
int settings_get_ShiftFontColor();
bool settings_is_HealthCustomAlgoritm();
int settings_get_HealhHeight();
bool is_time_to_shift();
bool settings_get_HealthSteps();
bool can_update_weather();

#if defined(PBL_HEALTH)
PEBBLE_HEALTH_METRIC settings_get_LeftHealthMetric();
PEBBLE_HEALTH_METRIC settings_get_RightHealthMetric();
#endif

#if defined (PBL_PLATFORM_APLITE)
bool settings_get_QT();
uint8_t settings_get_QTHourBegin();
uint8_t settings_get_QTHourEnd();
uint8_t settings_get_QTMinBegin();
uint8_t settings_get_QTMinEnd();
#endif
