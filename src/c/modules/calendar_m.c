#include <pebble.h>
#include "include/calendar_m.h"
#include "../utils/include/timeutils.h"


static Layer *s_this_layer;
static void prv_populate_this_layer(Layer *, GContext *);
//static char
static char* weekdays_abbr[] = {"пн", "вт", "ср", "чт", "пт", "сб"};
static char* special_day = "вс";
static int days[21];
static struct tm *ct;
static void prv_update_time();

void init_calendar_layer(GRect rect) {
  s_this_layer = layer_create(rect);
  prv_update_time();
  //get_raw_time_ptr(ct);
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

static void fill_dates() {
  //update_timer();
  prv_update_time();
  time_t now, previous_week_start, filler;
  now = time(NULL);

  //int current_date = ct->tm_mday;
  int current_week_day = ct->tm_wday;
  //int current_week = ct->tm_yday % 7 + 1;
  //int current_month = ct->tm_mon;
  struct tm *filler_t;
  previous_week_start = now - (6 + current_week_day) * SECONDS_PER_DAY;
  filler = previous_week_start;
  //next_week_end = now + (14 - current_week_day) * SECONDS_PER_DAY;
  filler_t = localtime(&filler);

  //nwe_t = localtime(&next_week_end);
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "prev monday is:%i", pws_t->tm_mday);
  for (int i = 0; i < 21; i++) {
    days[i] = filler_t->tm_mday;
    filler += SECONDS_PER_DAY;
    filler_t = localtime(&filler);
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "day [%i] is [%i]", i+1, days[i]);
  }


  //int this_month_days_passed = current_date -


}

static void prv_populate_this_layer(Layer *me, GContext *ctx) {
  //  static char* weekdays_abbr[] = {"пн", "вт", "ср", "чт", "пт", "сб"};
  //  static char* special_day = "вс";
   //
  //  static char* days[20];
  fill_dates();
   GRect bounds = layer_get_bounds(s_this_layer);
   int width = (bounds.size.w) / 7;
   for (int i = 0; i < 7; i++) {
     GRect pos = GRect(i * width, 0, width, 20);
     graphics_draw_text(ctx, i < 6 ? weekdays_abbr[i] : special_day, \
         fonts_get_system_font(FONT_KEY_GOTHIC_24), \
         pos, \
         GTextOverflowModeWordWrap, \
         GTextAlignmentCenter, \
         NULL);
   }

  //  for (int i = 0; i < 21; i++) {
  //    i < 10 ? snprintf(days[i], , "0%i", i) : xprintf(days[i], "%i", i);
  //  }
   //int height = 0;
   for (int iy = 0; iy < 3; iy ++) {
      for (int ix = 0; ix < 7; ix++) {
       char date_txt[3];
       snprintf(date_txt, sizeof(date_txt), "%d", days[iy * 7 + ix]);
     int x = ix * width;
     int y = iy  * 15+25;
     GRect pos = GRect(x, y, width, 20);
     graphics_draw_text(ctx, date_txt, \
         fonts_get_system_font(FONT_KEY_GOTHIC_18), \
         pos, \
         GTextOverflowModeWordWrap, \
         GTextAlignmentCenter, \
         NULL);
   }}
}
