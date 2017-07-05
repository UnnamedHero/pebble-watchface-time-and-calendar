#include <pebble.h>
#include "top-panel-layer.h"
#include "../modules/include/bluetooth.h"
#include "../modules/include/battery.h"

#define TOP_LATER_HEIGHT 20

static Layer *layer;
static const GRect bt_rect = GRect(0,0, 20, 20);
static GRect batt_rect;

static void populate_layer(Layer *, GContext *);

void init_top_panel_layer(GRect bounds) {
  layer = layer_create(bounds);
  batt_rect = GRect (bounds.size.w - 35, 0, 35, TOP_LATER_HEIGHT);
  layer_set_update_proc(layer, populate_layer);
  init_bluetooh(bt_rect);
  init_battery_layer(batt_rect);

  layer_add_child(layer, get_layer_bluetooth());
  layer_add_child(layer, get_layer_battery());
}

void deinit_top_panel_layer() {
  deinit_bluetooth();
  deinit_battery_layer();
  if (layer) {
    layer_destroy(layer);
  }
}


Layer* get_top_panel_layer() {
  return layer;
}

static void populate_layer(Layer *layer, GContext *ctx) {
  //graphics_draw_bitmap_in_rect(ctx, s_bt_icon, GRect(0,0, 20, 20));
  APP_LOG(APP_LOG_LEVEL_DEBUG, "redraw top-panel-layer");
 }
