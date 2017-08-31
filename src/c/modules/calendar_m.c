#include <pebble.h>
#include "include/calendar_m.h"
#include "../utils/include/timeutils.h"
#include "../settings.h"
#include "../3rdparty/locale_framework/localize.h"

static Layer *s_this_layer;
static void prv_populate_this_layer(Layer *, GContext *);

static int days[21];
static char calendar_values[28][5];
static struct tm *ct;
static void prv_update_time();

void init_calendar_layer(GRect rect) {
  s_this_layer = layer_create(rect);
  prv_update_time();
  layer_set_update_proc(s_this_layer, prv_populate_this_layer);

}

void deinit_calendar_layer() {
  if (s_this_layer) {
    layer_destroy(s_this_layer);
  }
}

static void prv_update_time() {
  time_t temp = time(NULL);
  ct = localtime(&temp);
}

Layer* get_layer_calendar() {
  return s_this_layer;
}

static int s_get_calendar_begin() {
  switch (settings_get_CalendarWeekView()) {
    case CAL_WV_PPC:
      return 14;
    case CAL_WV_PCN:
      return 7;
    case CAL_WV_CNN:
      return 0;
  }
  return 7;
}

static void fill_dates() {

  time_t now, previous_week_start, filler;
  now = time(NULL);
  prv_update_time();
  int current_week_day = ct->tm_wday;
  struct tm *filler_t;//;, *now_test;
  
  previous_week_start = now - (s_get_calendar_begin() + current_week_day) * SECONDS_PER_DAY;
  if (!settings_get_SundayFirst()) {
    previous_week_start += SECONDS_PER_DAY;
    if (current_week_day ==0) {
      previous_week_start -= 7 * SECONDS_PER_DAY;
    }
  }
  
  filler = previous_week_start;
  filler_t = localtime(&filler);
  for (int i = 0; i < 21; i++) {
    days[i] = filler_t->tm_mday;
    filler += SECONDS_PER_DAY;
    filler_t = localtime(&filler);
  }
}

static void populate_calendar_values() {
  fill_dates();
  char* weekdays_abbr[] = {_("mo"), _("tu"), _("we"), _("th"), _("fr"), _("sa")};
  char* special_day = _("su");

  for (int row = 0; row < 4; row ++) { //4 rows, days of week + 3 weeks
    for (int col = 0; col < 7; col ++) { // 7 days in a week
      if (row == 0) { //filling first row
        if (settings_get_SundayFirst()) {

          col == 0 ? snprintf(calendar_values[col], sizeof(calendar_values[col]), "%s", special_day) : \
                     snprintf(calendar_values[col], sizeof(calendar_values[col]), "%s", weekdays_abbr[col - 1]);
        } else {
          col < 6 ? snprintf(calendar_values[col], sizeof(calendar_values[col]), "%s", weekdays_abbr[col]) : \
                    snprintf(calendar_values[col], sizeof(calendar_values[col]), "%s", special_day);
        }
      } else {
        int index = row * 7 + col;
        snprintf(calendar_values[index], sizeof(calendar_values[index]), "%d", days[index - 7]);
      }
    }
  }
}

static void prv_set_light_theme(GContext *ctx) {
    graphics_context_set_fill_color(ctx, GColorWhite);
    graphics_context_set_text_color(ctx, GColorBlack);
}

static void prv_set_dark_theme(GContext *ctx) {
    graphics_context_set_fill_color(ctx, GColorBlack);
    graphics_context_set_text_color(ctx, GColorWhite);
}

static void prv_populate_this_layer(Layer *me, GContext *ctx) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Draw: CALENDAR");
  populate_calendar_values();
  GRect bounds = layer_get_bounds(s_this_layer);

  graphics_context_set_fill_color(ctx, GColorWhite);

  int width = (bounds.size.w) / 7;
  prv_update_time();
  int current_week_day_abbr_index;
  if (settings_get_SundayFirst()) {
   current_week_day_abbr_index = ct->tm_wday;
  } else {
   current_week_day_abbr_index = ct->tm_wday == 0 ? 6 : ct->tm_wday - 1;
 }
  int current_date_index =current_week_day_abbr_index + s_get_calendar_begin() + 7;
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "DOW: %d, date: %d", ct->tm_wday, ct->tm_mday);
  GRect backgrnd = GRect (bounds.origin.x + 1, bounds.origin.y, bounds.size.w - 6, bounds.size.h - 1);
  graphics_fill_rect(ctx, backgrnd, 0, GCornerNone);
  prv_set_dark_theme(ctx);
  for (int iy = 0; iy < 4; iy ++) {
     for (int ix = 0; ix < 7; ix++) {
      int x = ix * width;
      int y = iy  * 18;
      GRect fill = iy == 0 ? GRect (x, y, width, 18) : GRect (x + 1, y + 1, width -1 , 17);
      GRect pos = GRect(x, y - 3, width, 18);
      int index = iy * 7 + ix;
      GFont font = fonts_get_system_font(FONT_KEY_GOTHIC_18);
      if (index == current_week_day_abbr_index) { 
        if (settings_get_CalendarInvertWeekDay()) {
          prv_set_light_theme(ctx);
        }
        if (settings_get_CalendarBoldWeekDay()) {
          font = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
        }
      }

      if (index == current_date_index) {
        if (settings_get_CalendarInvertToday()) {
          prv_set_light_theme(ctx);  
        }      
        if (settings_get_CalendarBoldToday()) {
           font = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
        }        
      }

      graphics_fill_rect(ctx, fill, 0, GCornerNone);
      graphics_draw_text(ctx, calendar_values[index], \
          font, \
          pos, \
          GTextOverflowModeWordWrap, \
          GTextAlignmentCenter, \
          NULL);
       prv_set_dark_theme(ctx);
    }

 }
}

void update_layer_calendar() {
  layer_mark_dirty(s_this_layer);
}
