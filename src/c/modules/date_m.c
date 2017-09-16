#include <pebble.h>
#include "include/date_m.h"
#include "../settings.h"
#include "../utils/include/timeutils.h"
#include "../utils/include/textutils.h"

static Layer *s_this_layer;
static void prv_populate_date_layer(Layer *, GContext *);

void init_date_layer(GRect rect) {
  s_this_layer = layer_create(rect);
  layer_set_update_proc(s_this_layer, prv_populate_date_layer);
}

void deinit_date_layer() {
  if (s_this_layer) {
    layer_destroy(s_this_layer);
  }
}

Layer* get_layer_date() {
  return s_this_layer;
}

static void prv_populate_date_layer(Layer *me, GContext *ctx) {
  #if defined (DEBUG) 
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Draw: DATE");
  #endif  
  settings_get_theme(ctx);
  static char date_txt[33];
  get_current_date(settings_get_DateFormat(), date_txt);
  GFont font = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  GRect pretty_rect = get_pretty_rect_for_text(layer_get_bounds(me), font);
  graphics_draw_text(ctx, date_txt, \
      font, \
      pretty_rect, \
      GTextOverflowModeWordWrap, \
      GTextAlignmentCenter, \
      NULL);
}

void update_layer_date() {
  layer_mark_dirty(s_this_layer);
}
