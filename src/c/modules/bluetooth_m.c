#include <pebble.h>
#include "include/bluetooth_m.h"
#include "../settings.h"
#include "../utils/include/timeutils.h"
#include "../utils/include/vibe.h"

//static GBitmap *s_bt_icon;
static GFont statuses_font;
static Layer *this_layer;

static void prv_bt_connection_status(bool state);
static void prv_populate_bt_layer(Layer *, GContext *);
static bool init = false;
static bool bt_connected = false;

void init_bluetooh_layer(GRect rect) {
  this_layer = layer_create(rect);
  layer_set_update_proc(this_layer, prv_populate_bt_layer);
  connection_service_subscribe((ConnectionHandlers) {
    .pebble_app_connection_handler = prv_bt_connection_status,
    .pebblekit_connection_handler = NULL,
  });
  statuses_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_STATUSES_18));
  prv_bt_connection_status(connection_service_peek_pebble_app_connection());
  init = true;
}

static void prv_bt_connection_status(bool state) {
  
  bt_connected = state;
  if (can_vibrate() && init) {
    state ? do_vibrate (settings_get_VibrateConnectedType()) : do_vibrate(settings_get_VibrateDisconnectedType());
  }

  layer_mark_dirty(this_layer);
}

static void prv_populate_bt_layer(Layer *me, GContext *ctx) {
  #if defined (DEBUG) 
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Draw: STATUS LAYER");
  #endif  
  
  settings_get_theme(ctx);
  graphics_draw_text(ctx, bt_connected ? "B" : "A" , \
    statuses_font, \
    GRect (0, 0, 20, 20), \
    GTextOverflowModeWordWrap, \
    GTextAlignmentCenter, \
    NULL);
  if (settings_get_WeatherAPIKeyStatus() != API_OK) {
    graphics_draw_text(ctx, "C" , \
    statuses_font, \
    GRect (20, 0, 20, 20), \
    GTextOverflowModeWordWrap, \
    GTextAlignmentCenter, \
    NULL);

  }
  //graphics_draw_bitmap_in_rect(ctx, s_bt_icon, GRect(0,0, 20, 20));

//  APP_LOG(APP_LOG_LEVEL_DEBUG, "REDRAW: BT-layer");
}


void deinit_bluetooth_layer() {
  connection_service_unsubscribe();
  fonts_unload_custom_font(statuses_font);
  // if (s_bt_icon) {
  //   gbitmap_destroy(s_bt_icon);
  // }
  if (this_layer) {
    layer_destroy(this_layer);
  }
}

Layer* get_layer_bluetooth() {
  return this_layer;
}


bool is_bt_connected() {
  return bt_connected;
}