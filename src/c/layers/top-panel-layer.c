#include <pebble.h>
#include "top-panel-layer.h"
#include "../modules/include/bluetooth.h"

static Layer *layer;
//static GBitmap *s_bt_icon;
//static const GRect bt_rect = GRect(0,0, 20, 20);

static void populate_layer(Layer *, GContext *);
//static void top_layer_update_handler(UPDATE_FLAG);


void init_top_panel_layer(GRect bounds) {
//  s_bt_connected = gbitmap_create_with_resource(RESOURCE_ID_BLUETOOTH_CONNECTED);
  layer = layer_create(GRect(0, 0, bounds.size.w, TOP_LATER_HEIGHT));
  layer_set_update_proc(layer, populate_layer);
  init_bluetooh();

  layer_add_child(layer, get_layer_bluetooth());
  // connection_service_subscribe((ConnectionHandlers) {
  //   .pebble_app_connection_handler = prv_bt_connection_status,
  //   .pebblekit_connection_handler = NULL,
  // });
  //
  // prv_bt_connection_status(connection_service_peek_pebble_app_connection());
}

void deinit_top_panel_layer() {
  deinit_bluetooth();
  if (layer) {
    layer_destroy(layer);
  }
  // if (s_bt_icon) {
  //   gbitmap_destroy(s_bt_icon);
  // }
}
//PLATFORM_APLITE

Layer* get_top_panel_layer() {
  return layer;
}

static void populate_layer(Layer *layer, GContext *ctx) {
  //graphics_draw_bitmap_in_rect(ctx, s_bt_icon, GRect(0,0, 20, 20));
  APP_LOG(APP_LOG_LEVEL_DEBUG, "redraw top-panel-layer");
  //graphics_draw_bitmap_in_rect(ctx, s_bt_connected, GRect(21,0, 20, 20));


// static void prv_bt_connection_status(bool state) {
//    if (s_bt_icon) {
//      gbitmap_destroy(s_bt_icon);
//    }
// //   int res_id = state ? RESOURCE_ID_BLUETOOTH_CONNECTED : RESOURCE_ID_BLUETOOTH_LOST;
//    s_bt_icon = gbitmap_create_with_resource(get_bt_icon_resid());
//   layer_mark_dirty(layer);
 }

// static void top_layer_update_handler(UPDATE_FLAG f) {
//     layer_mark_dirty(layer);
// }
