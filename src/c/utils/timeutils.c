#include <pebble.h>
#include "include/timeutils.h"
#include "../settings.h"
#include "include/ticktimerhelper.h"
#include "../modules/include/weather_m.h"
#include "../3rdparty/locale_framework/localize.h"

static struct tm currentTime = {0};

static int get_period_seconds(PERIOD per);

struct tm* get_Time() {
  return &currentTime;
}

static void prv_utils_handler(struct tm* tick_time) {  
  currentTime = *tick_time;
  layer_mark_dirty(get_layer_weather());
}

void init_time_utils() {  
  time_t temp = time(NULL);
  currentTime = *(localtime(&temp));
  ticktimerhelper_register_clock(prv_utils_handler);
  }

#if defined (PBL_PLATFORM_APLITE)

bool aplite_quiet_time_is_active() { 
  int current_min_of_a_day = get_mins(currentTime.tm_hour, currentTime.tm_min);  
  int qtmins_begin = get_mins(settings_get_QTHourBegin(), settings_get_QTMinBegin());
  int qtmins_end = get_mins(settings_get_QTHourEnd(), settings_get_QTMinEnd());  
  return is_within_range(current_min_of_a_day, qtmins_begin, qtmins_end);
}
#endif

bool is_quiet_time() {
#if defined (PBL_PLATFORM_APLITE)
  bool qt = settings_get_QT() && aplite_quiet_time_is_active();
#else
  bool qt = quiet_time_is_active();
#endif
  return qt;
}

void get_currect_time(DT_FORMAT dtf, char *buffer) {
  char d_buffer[32];
  char format[12];
  switch (dtf) {
    case YYYY_MM_DD:
      strcpy(format, "%Y.%m.%d");
      break;
    case CLOCK_FORMAT:
      strcpy(format, settings_get_ClockFormat());
      break;
    case DT_CLOCK_SECS:
      strcpy(format, settings_get_ClockFormat());
      strcat(format, ":%S");
      break;
    }
  strftime(d_buffer, sizeof(d_buffer), format, &currentTime);
  strcpy(buffer, d_buffer);
}

void get_current_date(char *format, char *out) {
  char* rus_months[] = { _("jan"), _("feb"), _("mar"), _("apr"), _("may"), _("jun"), _("jul"), _("aug"), _("sep"), _("oct"), _("nov"), _("dec") };
  const size_t out_size = 32;

  const char* locale_str = i18n_get_system_locale();
  const bool locale_ru = strcmp(locale_str, "ru_RU") == 0;
  const bool is_special_format = strcmp(format, "%B %d, %Y") == 0;
  if (locale_ru && is_special_format) {
    snprintf(out, out_size, "%d %s, %d", currentTime.tm_mday, rus_months[currentTime.tm_mon], currentTime.tm_year + 1900);
  } else {
    strftime(out, out_size, format, &currentTime);
  } 
}

bool is_time_to(uint32_t timestamp, PERIOD period) {
  int secs_to_wait = get_period_seconds(period);
  #if defined (DEBUG)
    APP_LOG(APP_LOG_LEVEL_DEBUG, "requested period %d for %d secs", period, secs_to_wait);
  #endif
  uint32_t elapsed = timestamp + secs_to_wait;
  struct tm _time = *get_Time();
  uint32_t gmtoffset = _time.tm_gmtoff;
  uint32_t current = (uint32_t)mktime(&_time) + gmtoffset;
  #if defined (DEBUG)
    APP_LOG(APP_LOG_LEVEL_DEBUG, "elapsed %li <=> current %li, delta: %li", elapsed, current, current - elapsed);
  #endif
  return elapsed <= current;
}

static int get_period_seconds(PERIOD per) {  
  switch (per) {
    case P_15MIN:
      return 900;
    case P_30MIN:
      return 1800;
    case P_1H:
      return 3600;
    case P_3H:
      return 10800;
    case P_6H:
      return 21600;
    default:
      return 1800;
  }
}
