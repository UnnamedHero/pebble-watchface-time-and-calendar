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

typedef struct state state, *st;
typedef void (*accel_handler)();

struct state {  
  accel_handler handler;
};

//static bool main_window_active = true;
static AppTimer *s_timeout_timer;
static void prv_timer_timeout_handler(void*);

static state *current_state;
static state clock_and_weather, clock_and_weather_forecast, no_forecast, \
              seconds_clock, seconds_clock_showing;

static void prv_init_timer(uint8_t timeout);
static void prv_set_init_state();

// static void toggle_windows() {
//   bool now = main_window_active;
//   const bool animated = true;
//   //window_stack_push(get_time_window(), animated);
//   if (now) { 
//     Window *fw = get_forecast_window();
    
//     window_stack_push(fw, animated);
//     uint8_t swback_timeout = settings_get_SwitchBackTimeout();
//     if (swback_timeout > 0 && swback_timeout < 60) {
//       s_timeout_timer = app_timer_register(swback_timeout * 1000, prv_timer_timeout_handler, NULL);
//     }
//   } else {
//     app_timer_cancel(s_timeout_timer);
//     window_stack_push(get_time_window(), animated);
//   }
//   main_window_active = !now;
// }


void settings_update_handler() {
  prv_set_init_state();
  time_window_force_redraw();
}

static void accel_tap_handler(AccelAxisType axis, int32_t direction) { 
/*   if ((settings_get_WeatherStatus() == WEATHER_OK || settings_get_WeatherStatus() == WEATHER_LOCATION_ERROR) &&\
      settings_get_ForecastEnabled()) {
    toggle_windows();
  }
 */  current_state->handler();
}

static void cw_accel_handler() {
  #if defined (DEBUG)
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Handling cw_accel_handler");
  #endif  
  window_stack_push(get_forecast_window(), false);      
  current_state = &clock_and_weather_forecast;
  prv_init_timer(settings_get_SwitchBackTimeout());
}

static void cw_f_accel_handler() {
  #if defined (DEBUG)
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Handling cw_f_accel_handler");
  #endif  
  window_stack_push(get_time_window(), true);
  app_timer_cancel(s_timeout_timer);
  prv_set_init_state();
}

static void cw_no_forecast_handler() {
  #if defined (DEBUG)
    APP_LOG(APP_LOG_LEVEL_DEBUG, "cw_no_forecast handler");
  #endif
}

static void seconds_clock_handler() {
  settings_set_ClockShowSeconds_showing();
  current_state = &seconds_clock_showing;  
  prv_init_timer(settings_get_SwitchBackTimeoutSeconds());
  reload_timer();
}

static void seconds_clock_showing_handler() {
  settings_set_ClockShowSeconds_enabled();
  app_timer_cancel(s_timeout_timer);
  prv_set_init_state();
  reload_timer();
}

static void prv_init_states() {
  clock_and_weather = (state){
    .handler = cw_accel_handler
  };

  clock_and_weather_forecast = (state){
    .handler = cw_f_accel_handler
  };

  no_forecast = (state) {
    .handler = cw_no_forecast_handler
  };

  seconds_clock = (state) {
    .handler = seconds_clock_handler
  };

  seconds_clock_showing = (state) {
    .handler = seconds_clock_showing_handler
  };
}

static void prv_set_init_state() {
  if (settings_get_WeatherStatus() == WEATHER_DISABLED) {
    if (settings_get_ClockShowSeconds() == SEC_ENABLED) {
      current_state = &seconds_clock;
      return;
    }

    if (settings_get_ClockShowSeconds() == SEC_SHOWING) {
      current_state = &seconds_clock_showing;
      return;
    }
  }

  if (!settings_get_ForecastEnabled() || settings_get_WeatherStatus() == WEATHER_DISABLED) {
    current_state = &no_forecast;
    return;
  }
  current_state = &clock_and_weather;
  //reload_timer();
}

static void prv_init() {
  start_ticktimerhelper();
  init_message_system();
  init_vibrate();
  init_settings(settings_update_handler);
  prv_init_states();
  prv_set_init_state();

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

static void prv_init_timer(uint8_t swback_timeout) {
  if (swback_timeout > 0) {
    s_timeout_timer = app_timer_register(swback_timeout * 1000, prv_timer_timeout_handler, NULL);
  }  
}

static void prv_timer_timeout_handler (void *context) {  
  current_state->handler();
}

int main(void) {
  locale_init();
  prv_init();
  app_event_loop();
  prv_deinit();
}
