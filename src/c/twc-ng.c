#include <pebble.h>
#include "twc-ng.h"
#include "settings.h"
#include "windows/include/time-window.h"
#include "windows/include/forecast-window.h"
#include "utils/include/timeutils.h"
#include "utils/include/vibe.h"
#include "utils/include/messagesystem.h"
#include "utils/include/ticktimerhelper.h"
#include "3rdparty/locale_framework/localize.h"
#include "utils/include/states.h"

void settings_update_handler() {
  #if defined (DEBUG)
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Update Settings in twc-ng");
  #endif
  //prv_set_init_state();
  notify_settings_changed();
  time_window_force_redraw();
}

static void prv_init() {
  start_ticktimerhelper();
  init_time_utils();
  init_message_system();
  init_vibrate();
  init_settings(settings_update_handler);
  init_states();
//  prv_set_init_state();

  init_time_window();
  init_forecast_window();

  const bool animated = true;
  //window_stack_push(get_forecast_window(), !animated);
  window_stack_push(get_time_window(), !animated);
  
}

static void prv_deinit(void) {
 stop_ticktimerhelper();  
 deinit_time_window();
 deinit_forecast_window();
 deinit_message_system();
}


int main(void) {
  locale_init();
  prv_init();
  app_event_loop();
  prv_deinit();
}
