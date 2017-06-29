#include <pebble.h>
#include "top-panel-layer.h"


static Layer *layer;

static GBitmap *s_bt_connected;

static void populate_layer(Layer *, GContext *);

void init_top_panel_layer(GRect bounds) {
  s_bt_connected = gbitmap_create_with_resource(RESOURCE_ID_BLUETOOTH_CONNECTED);
  layer = layer_create(GRect(0, 0, bounds.size.w, TOP_LATER_HEIGHT));
  layer_set_update_proc(layer, populate_layer);
}

void deinit_top_panel_layer() {
  if (layer) {
    layer_destroy(layer);
  }
    gbitmap_destroy(s_bt_connected);
}

static void populate_layer(Layer *layer, GContext *ctx) {
  graphics_draw_bitmap_in_rect(ctx, s_bt_connected, GRect(0,0, 20, 20));
  graphics_draw_bitmap_in_rect(ctx, s_bt_connected, GRect(21,0, 20, 20));
}

Layer* get_top_panel_layer() {
  return layer;
}
