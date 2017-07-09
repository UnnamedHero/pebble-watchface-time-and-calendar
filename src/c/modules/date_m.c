#include <pebble.h>
#include "include/date_m.h"
#include "../settings.h"
#include "../utils/include/timeutils.h"

static Layer *s_this_layer;
static void prv_populate_date_layer(Layer *, GContext *);
//static char

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
  graphics_draw_text(ctx, get_currect_time(YYYY_MM_DD), \
      fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD), \
      layer_get_bounds(s_this_layer), \
      GTextOverflowModeWordWrap, \
      GTextAlignmentCenter, \
      NULL);
}
