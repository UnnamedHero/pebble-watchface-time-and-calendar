#include <pebble.h>
#include "include/battery_m.h"
#include "../settings.h"

static Layer *s_battery_layer;
static GFont statuses_font;
static BatteryChargeState s_battery_level;
static void prv_battery_handler(BatteryChargeState);
static void prv_populate_battery_layer(Layer *, GContext *);

void init_battery_layer(GRect rect) {
  s_battery_layer = layer_create(rect);
  layer_set_update_proc(s_battery_layer, prv_populate_battery_layer);
  battery_state_service_subscribe(prv_battery_handler);
  prv_battery_handler(battery_state_service_peek());
  statuses_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_STATUSES_18));
}

void deinit_battery_layer() {
    battery_state_service_unsubscribe();
    layer_destroy(s_battery_layer);
    fonts_unload_custom_font(statuses_font);
}

static void prv_battery_handler(BatteryChargeState state) {

if ((state.charge_percent != s_battery_level.charge_percent) || \
    (state.is_charging != s_battery_level.is_charging) || \
    (state.is_plugged != s_battery_level.is_plugged)) {
      APP_LOG(APP_LOG_LEVEL_DEBUG, "battery_changed");
      s_battery_level = state;
      layer_mark_dirty(s_battery_layer);
  }
}

static void prv_populate_battery_layer(Layer *me, GContext *ctx) {
  #if defined (DEBUG) 
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Draw: BATTERY");
  #endif
  static const char* battery_bar[11] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", ":"};
  settings_get_theme(ctx);
  static char percent_text[5];

  GRect bb = layer_get_bounds(me);
  GRect percent_text_rect = GRect (0, 0, 34, bb.size.h);
  GRect battery_bar_rect = GRect (34, -2, 24, bb.size.h);

  int battery_bar_index = s_battery_level.charge_percent == 0 ? 0 : s_battery_level.charge_percent / 10;
  snprintf(percent_text, sizeof(percent_text), "%d%%", s_battery_level.charge_percent);
  graphics_draw_text(ctx, percent_text, \
  fonts_get_system_font(FONT_KEY_GOTHIC_18), \
    percent_text_rect, \
    GTextOverflowModeWordWrap, \
    GTextAlignmentRight, NULL);
  graphics_draw_text(ctx, battery_bar[battery_bar_index], \
    statuses_font, \
    battery_bar_rect, \
    GTextOverflowModeWordWrap, \
    GTextAlignmentLeft, NULL);  
}

Layer* get_layer_battery() {
    return s_battery_layer;
}
