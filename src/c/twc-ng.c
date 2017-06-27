#include <pebble.h>
#include "twc-ng.h"
//#include "localize.h"
#include "settings.h"


static Window *s_main_window;
static TextLayer *s_time_layer;



//---------bitmaps
static BitmapLayer *s_top_panel_layer_1;

static GBitmap *s_bt_connected;

// static void prv_select_click_handler(ClickRecognizerRef recognizer, void *context) {
//   text_layer_set_text(s_time_layer, "Select");
// }
//
// static void prv_up_click_handler(ClickRecognizerRef recognizer, void *context) {
//   text_layer_set_text(s_text_layer, "Up");
// }
//
// static void prv_down_click_handler(ClickRecognizerRef recognizer, void *context) {
//   text_layer_set_text(s_text_layer, "Down");
// }
//
// static void prv_click_config_provider(void *context) {
//   window_single_click_subscribe(BUTTON_ID_SELECT, prv_select_click_handler);
//   window_single_click_subscribe(BUTTON_ID_UP, prv_up_click_handler);
//   window_single_click_subscribe(BUTTON_ID_DOWN, prv_down_click_handler);
// }



// Save the settings to persistent storage
// static void prv_save_settings() {
//   save_settings();
//   // Update the display based on new settings
//   prv_update_display();
// }

static void prv_update_display() {
  update_time();
}


static void tick_handler(struct tm *tick_time, TimeUnits units_chanhed) {
  update_time();
}

static void prv_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

//-----bitmaps
  s_bt_connected = gbitmap_create_with_resource(RESOURCE_ID_BLUETOOTH_CONNECTED);

//----bitmap layers
  s_top_panel_layer_1 = bitmap_layer_create((GRect(0, 0, 20, 20)));

//----text layers
  s_time_layer = text_layer_create(GRect(0, 72, bounds.size.w, 20));

//----set bitmaps
  bitmap_layer_set_bitmap(s_top_panel_layer_1, s_bt_connected);

//----set text
  text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

//---adding layers
  layer_add_child(window_layer, bitmap_layer_get_layer(s_top_panel_layer_1));
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  prv_update_display();
}

static void prv_window_unload(Window *window) {
  text_layer_destroy(s_time_layer);
  gbitmap_destroy(s_bt_connected);
  bitmap_layer_destroy(s_top_panel_layer_1);
}

void settings_update_handler() {
  prv_update_display();
}

static void prv_init() {
  //locale_init();
  // int (*handler_ptr)(void) = NULL;
  // handler_ptr = &settings_update_handler;
  // init_settings(handler_ptr);
  init_settings(settings_update_handler);

  s_main_window = window_create();
//  window_set_click_config_provider(s_window, prv_click_config_provider);
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = prv_window_load,
    .unload = prv_window_unload,
  });
  const bool animated = true;

  // Open AppMessage connection


  window_stack_push(s_main_window, animated);
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  update_time();
}

static void prv_deinit(void) {
  window_destroy(s_main_window);
}

int main(void) {
  prv_init();



  app_event_loop();
  prv_deinit();
}

static void update_time() {
    time_t temp = time(NULL);
    struct tm *tick_time = localtime(&temp);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "time format:[%s]", settings_get_clockformat());
    static char s_buffer[8];//    char format[5];
    strftime(s_buffer, sizeof(s_buffer), settings_get_clockformat(), tick_time);
    text_layer_set_text(s_time_layer, s_buffer);
}
