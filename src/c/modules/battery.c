#include <pebble.h>
#include "include/battery.h"

static Layer *s_battery_layer;

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
}

static void prv_battery_handler(BatteryChargeState state) {
  s_battery_level = state;
  layer_mark_dirty(s_battery_layer);
}

static void prv_populate_battery_layer(Layer *me, GContext *ctx) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "REDRAW: Battery Layer");
    graphics_context_set_fill_color(ctx, GColorWhite);
    graphics_context_set_stroke_color(ctx, GColorWhite);
    GRect bb = layer_get_bounds(me);
    int width = s_battery_level.charge_percent * 3 / 10;
    APP_LOG(APP_LOG_LEVEL_DEBUG, "State is %d, width is %d", s_battery_level.charge_percent, width);
    GRect battery_inner_bar = GRect(2, 2, width, bb.size.h -4);
//    GRect battery_pin = GRect(bb.size.w - 5, bb.origin.y -10, 3, 10);
    graphics_draw_rect(ctx, layer_get_bounds(me));
    graphics_fill_rect(ctx, battery_inner_bar, 0, GCornerNone);
    //graphics_draw_text(ctx, "100\0",fonts_get_system_font(FONT_KEY_GOTHIC_14), GRect(12, 4, 25, 10), GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
    //graphics_draw_rect(ctx, battery_pin);

}
Layer* get_layer_battery() {
    return s_battery_layer;
}
