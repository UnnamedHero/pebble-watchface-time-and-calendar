#include <pebble.h>
#include "twc-ng.h"

ClaySettings settings;

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
static void prv_default_settings() {
  settings.ClockFormat = true;
}

static void prv_load_settings() {
  // Load the default settings
  prv_default_settings();
  // Read settings from persistent storage, if they exist
  persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
}

// Save the settings to persistent storage
static void prv_save_settings() {
  persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
  // Update the display based on new settings
  prv_update_display();
}

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

static void prv_inbox_received_handler(DictionaryIterator *iter, void *context) {
  Tuple *is_clock_24h_t = dict_find(iter, MESSAGE_KEY_ClockFormat);
  if(is_clock_24h_t) {
    settings.ClockFormat= is_clock_24h_t->value->int32 == 1;
  }
  prv_save_settings();
}
static void prv_init(void) {
  prv_load_settings();

  app_message_register_inbox_received(prv_inbox_received_handler);
  app_message_open(128, 128);

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

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", s_main_window);

  app_event_loop();
  prv_deinit();
}

static void update_time() {
    time_t temp = time(NULL);
    struct tm *tick_time = localtime(&temp);

    static char s_buffer[8];
    strftime(s_buffer, sizeof(s_buffer), settings.ClockFormat ?
      "%H:%M" : "%I:%M", tick_time);
    text_layer_set_text(s_time_layer, s_buffer);
}
