#include <pebble.h>
#include "include/time_m.h"
#include "../settings.h"
#include "../utils/include/timeutils.h"

static Layer *s_this_layer;
static void prv_populate_time_layer(Layer *, GContext *);

void init_time_layer(GRect rect) {
  s_this_layer = layer_create(rect);
  layer_set_update_proc(s_this_layer, prv_populate_time_layer);
}

void deinit_time_layer() {
  if (s_this_layer) {
    layer_destroy(s_this_layer);
  }
}

void time_layer_update_time() {
  layer_mark_dirty(s_this_layer);
}


Layer* get_layer_time() {
  return s_this_layer;
}

static void prv_populate_time_layer(Layer *me, GContext *ctx) {

  graphics_draw_text(ctx, get_currect_time(CLOCK_FORMAT), \
      fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD), \
      layer_get_bounds(s_this_layer), \
      GTextOverflowModeWordWrap, \
      GTextAlignmentCenter, \
      NULL);
}
