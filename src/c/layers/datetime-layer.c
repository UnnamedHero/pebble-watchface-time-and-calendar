#include <pebble.h>
#include "include/datetime-layer.h"
#include "../modules/include/date.h"
#include "../modules/include/time_m.h"

static Layer* dt_layer;


static void prv_populate_dt_layer(Layer *, GContext *);

void init_datetime_layer(GRect rect) {
  //GRect date_rect = GRect ()
  dt_layer = layer_create(rect);
  layer_set_update_proc(dt_layer, prv_populate_dt_layer);
  GRect date_rect = GRect(0, 0, rect.size.w, 20);
  GRect time_rect = GRect(0, 10, rect.size.w, 50);

  init_date_layer(date_rect);
  init_time_layer(time_rect);

  layer_add_child(dt_layer, get_layer_date());
  layer_add_child(dt_layer, get_layer_time());
}

void deinit_datetime_layer() {
  deinit_date_layer();
  deinit_time_layer();
  if (dt_layer) {
    layer_destroy(dt_layer);
  }
}

Layer* get_datetime_layer() {
  return dt_layer;
}

static void prv_populate_dt_layer(Layer *me, GContext *ctx) {

}

void layer_update_time() {
  time_layer_update_time();
}
