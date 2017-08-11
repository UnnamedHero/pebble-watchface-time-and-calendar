#include <pebble.h>
#include "include/time_m.h"
#include "../settings.h"
#include "../utils/include/timeutils.h"
#include "../utils/include/textutils.h"

static Layer *s_this_layer;
static GFont s_tfont;
static void prv_populate_time_layer(Layer *, GContext *);

void init_time_layer(GRect rect) {
  s_this_layer = layer_create(rect);
  s_tfont = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_TIME_BOLD_54));
  layer_set_update_proc(s_this_layer, prv_populate_time_layer);
}

void deinit_time_layer() {
  if (s_this_layer) {
    layer_destroy(s_this_layer);
  }
  fonts_unload_custom_font(s_tfont);
}

void update_layer_time() {
  layer_mark_dirty(s_this_layer);
}


Layer* get_layer_time() {
  return s_this_layer;
}

static void prv_populate_time_layer(Layer *me, GContext *ctx) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Draw: TIME");
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "memory: %d", heap_bytes_free());
  //GFont font = fonts_get_system_font(FONT_KEY_ROBOTO_BOLD_SUBSET_49);
  GRect pretty_rect = get_pretty_rect_for_text(layer_get_bounds(me), s_tfont);
  static char time_txt[33];
  get_currect_time(CLOCK_FORMAT, time_txt);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "time to show: %s", time_txt);
  graphics_draw_text(ctx, time_txt, \
      s_tfont, \
      layer_get_bounds(me), \
      GTextOverflowModeWordWrap, \
      GTextAlignmentCenter, \
      NULL);
}
