#include <pebble.h>
#include "include/battery_m.h"

static Layer *s_battery_layer;
static GBitmap *s_battery_state_icon;

static BatteryChargeState s_battery_level;
static void prv_battery_handler(BatteryChargeState);
static void prv_populate_battery_layer(Layer *, GContext *);

void init_battery_layer(GRect rect) {
  s_battery_layer = layer_create(rect);
  layer_set_update_proc(s_battery_layer, prv_populate_battery_layer);
  battery_state_service_subscribe(prv_battery_handler);
  prv_battery_handler(battery_state_service_peek());
}

void deinit_battery_layer() {
    battery_state_service_unsubscribe();
    layer_destroy(s_battery_layer);
    if (s_battery_state_icon) {
      gbitmap_destroy(s_battery_state_icon);
     APP_LOG(APP_LOG_LEVEL_DEBUG, "remove battery bitmap in deinit");
    }

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
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Draw: BATTERY");
  static char percent_text[5];
  int battery_bar_level;

  GRect bb = layer_get_bounds(me);
  GRect percent_text_rect = GRect (0, 0, 30, bb.size.h);
  GRect battery_bar_rect = GRect (30, 0, 15, bb.size.h);

  GRect battery_inner_bar_rect;

  if (s_battery_level.is_plugged || s_battery_level.is_charging) {
    battery_bar_level = bb.size.h;
    int res_id = s_battery_level.is_charging ? \
                RESOURCE_ID_BATTERY_CHARGING : \
                RESOURCE_ID_BATTERY_CHARGED;
    s_battery_state_icon = gbitmap_create_with_resource(res_id);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "create battery bitmap in redraw");
  } else {
    battery_bar_level = s_battery_level.charge_percent / 5; // 2 px per 10%
  }
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_context_set_stroke_color(ctx, GColorWhite);
  battery_inner_bar_rect = GRect(30, bb.size.h - battery_bar_level, 15, battery_bar_level);
  snprintf(percent_text, sizeof(percent_text), "%d%%", s_battery_level.charge_percent);
  graphics_draw_text(ctx, percent_text, \
  fonts_get_system_font(FONT_KEY_GOTHIC_18), \
    percent_text_rect, \
    GTextOverflowModeWordWrap, \
    GTextAlignmentCenter, NULL);
  graphics_draw_rect(ctx, battery_bar_rect);
  graphics_fill_rect(ctx, battery_inner_bar_rect, 0, GCornerNone);

  if (s_battery_level.is_plugged || s_battery_level.is_charging) {
    graphics_draw_bitmap_in_rect (ctx, s_battery_state_icon, battery_bar_rect);
  }
}

Layer* get_layer_battery() {
    return s_battery_layer;
}
