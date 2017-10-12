#include <pebble.h>
#include "include/timeutils.h"
#include "../settings.h"
#include "../3rdparty/locale_framework/localize.h"

int period_to_mins(PERIOD per);

#if defined (PBL_PLATFORM_APLITE)
static int prv_get_mins(int hour, int min) {
  return hour * MINUTES_PER_HOUR + min;
}

static bool prv_is_time_quiet(int begin, int end, int now) {
  bool within_24h = begin <= end;
  int min = within_24h ? begin : end;
  int max = !within_24h ? begin : end;
  bool between = (now >= min) && (now <= max);
  if (within_24h) {
    return within_24h && between;
  } else {
    bool not_equal_state = !(between || within_24h);
    return now == begin || now == end ? !not_equal_state : not_equal_state;
  } 
}

bool aplite_quiet_time_is_active() {
  struct tm *tick_time;
  time_t temp = time(NULL);
  tick_time = localtime(&temp);
  
  int current_min_of_a_day = prv_get_mins(tick_time->tm_hour, tick_time->tm_min);  
  int qtmins_begin = prv_get_mins(settings_get_QTHourBegin(), settings_get_QTMinBegin());
  int qtmins_end = prv_get_mins(settings_get_QTHourEnd(), settings_get_QTMinEnd());  
  return prv_is_time_quiet(qtmins_begin, qtmins_end, current_min_of_a_day);
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
  struct tm *tick_time;
  time_t temp = time(NULL);
  tick_time = localtime(&temp);
  char d_buffer[32];
  char format[12];
  switch (dtf) {
    case YYYY_MM_DD:
      strcpy(format, "%Y.%m.%d");
      break;
    case CLOCK_FORMAT:
      strcpy(format, settings_get_ClockFormat());
      break;
    }
  strftime(d_buffer, sizeof(d_buffer), format, tick_time);
  strcpy(buffer, d_buffer);
}

void get_current_date(char *format, char *out) {
  char* rus_months[] = { _("jan"), _("feb"), _("mar"), _("apr"), _("may"), _("jun"), _("jul"), _("aug"), _("sep"), _("oct"), _("nov"), _("dec") };
  const size_t out_size = 32;
  struct tm *tick_time;
  time_t temp = time(NULL);
  tick_time = localtime(&temp);

  const char* locale_str = i18n_get_system_locale();
  const bool locale_ru = strcmp(locale_str, "ru_RU") == 0;
  const bool is_special_format = strcmp(format, "%B %d, %Y") == 0;
  if (locale_ru && is_special_format) {
    snprintf(out, out_size, "%d %s, %d", tick_time->tm_mday, rus_months[tick_time->tm_mon], tick_time->tm_year + 1900);
  } else {
    strftime(out, out_size, format, tick_time);
  } 
}

bool is_time_to(uint32_t timestamp, PERIOD period) {
  int secs_to_wait = period_to_mins(period) * SECONDS_PER_MINUTE;
  uint32_t elapsed = timestamp + secs_to_wait;
//  APP_LOG(APP_LOG_LEVEL_DEBUG, "elapsed %li <=> current %li", elapsed, (uint32_t)time(NULL));
  return elapsed < (uint32_t)time(NULL);
}

int period_to_mins(PERIOD per) {
  int mins = 0;
  switch (per) {
    case P_15MIN:
      mins = 15;
      break;
    case P_30MIN:
      mins = 30;
      break;
    case P_1H:
      mins = 60;
      break;
    case P_3H:
      mins = 180;
      break;
     case P_6H:
      mins = 360;
      break;
  }
  return mins;
}