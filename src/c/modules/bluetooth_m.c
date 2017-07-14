#include <pebble.h>
#include "include/bluetooth_m.h"
#include "../settings.h"
#include "../utils/include/timeutils.h"
#include "../utils/include/vibe.h"

static GBitmap *s_bt_icon;
static Layer *this_layer;

static void prv_bt_connection_status(bool state);
static void prv_populate_bt_layer(Layer *, GContext *);

void init_bluetooh_layer(GRect rect) {
  this_layer = layer_create(rect);
  layer_set_update_proc(this_layer, prv_populate_bt_layer);
  connection_service_subscribe((ConnectionHandlers) {
    .pebble_app_connection_handler = prv_bt_connection_status,
    .pebblekit_connection_handler = NULL,
  });

  prv_bt_connection_status(connection_service_peek_pebble_app_connection());
}

static void prv_bt_connection_status(bool state) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "bt state change");
  gbitmap_destroy(s_bt_icon);
  int res_id = state ? RESOURCE_ID_BLUETOOTH_CONNECTED : RESOURCE_ID_BLUETOOTH_LOST;
  s_bt_icon = gbitmap_create_with_resource(res_id);
  if (!s_bt_icon) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Error allocation memory for BT_ICON");
  }
  if (can_vibrate()) {
    state ? do_vibrate (settings_get_VibrateConnectedType()) : do_vibrate(settings_get_VibrateDisconnectedType());
  }

  layer_mark_dirty(this_layer);
}

static void prv_populate_bt_layer(Layer *me, GContext *ctx) {
  graphics_draw_bitmap_in_rect(ctx, s_bt_icon, GRect(0,0, 20, 20));
//  APP_LOG(APP_LOG_LEVEL_DEBUG, "REDRAW: BT-layer");
}


void deinit_bluetooth_layer() {
  connection_service_unsubscribe();
  if (s_bt_icon) {
    gbitmap_destroy(s_bt_icon);
  }
  if (this_layer) {
    layer_destroy(this_layer);
  }
}

Layer* get_layer_bluetooth() {
  return this_layer;
}
