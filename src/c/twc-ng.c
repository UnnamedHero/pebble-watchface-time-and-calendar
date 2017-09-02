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


static bool main_window_active = true;
static AppTimer *s_timeout_timer;
static void prv_timer_timeout_handler(void*);

static void toggle_windows() {
  bool now = main_window_active;
  const bool animated = true;
  //window_stack_push(get_time_window(), animated);
  if (now) { 
    Window *fw = get_forecast_window();
    
    window_stack_push(fw, animated);
    uint8_t swback_timeout = settings_get_SwitchBackTimeout();
    if (swback_timeout > 0 && swback_timeout < 60) {
      s_timeout_timer = app_timer_register(swback_timeout * 1000, prv_timer_timeout_handler, NULL);
    }
  } else {
    app_timer_cancel(s_timeout_timer);
    window_stack_push(get_time_window(), animated);
  }
  main_window_active = !now;
}


void settings_update_handler() {
  time_window_force_redraw();
}

static void accel_tap_handler(AccelAxisType axis, int32_t direction) {  
  toggle_windows();
}

static void prv_init() {
  start_ticktimerhelper();
  init_message_system();
  init_vibrate();
  init_settings(settings_update_handler);


  init_time_window();
  init_forecast_window();

  const bool animated = true;
  //window_stack_push(get_forecast_window(), !animated);
  window_stack_push(get_time_window(), !animated);
  accel_tap_service_subscribe(accel_tap_handler);
}

static void prv_deinit(void) {
 stop_ticktimerhelper();  
 deinit_time_window();
 deinit_forecast_window();
 deinit_message_system();
}

static void prv_timer_timeout_handler (void *context) {
  toggle_windows();
}

int main(void) {
  //APP_LOG(APP_LOG_LEVEL_DEBUG, "Locale: %s", i18n_get_system_locale());
  locale_init();
  prv_init();
  app_event_loop();
  prv_deinit();
}
