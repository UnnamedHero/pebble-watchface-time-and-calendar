#include <pebble.h>
#include "include/states.h"
#include "../settings.h"
#include "include/ticktimerhelper.h"
#include "../windows/include/time-window.h"
#include "../windows/include/forecast-window.h"

static int shakes = 0;
typedef struct state state, *st;
typedef void (*accel_handler)();

struct state {  
  accel_handler handler;
};

static AppTimer *s_timeout_timer;
static void prv_timer_timeout_handler(void*);
static void prv_timer_shake_handler(void*);

static state *current_state;
static state clock_and_weather, clock_and_weather_forecast, no_forecast, \
              seconds_clock, seconds_clock_showing;

static void prv_init_timer(uint8_t timeout);
static void prv_set_init_state();
static void prv_init_states();

static void manage_shakes() {
  if (!settings_get_ShakeTwice()) {
    current_state->handler();
  }
  switch (shakes) {
    case 0:
      if (current_state == &clock_and_weather_forecast || current_state == &seconds_clock_showing) {
        #if defined (DEBUG)
          APP_LOG(APP_LOG_LEVEL_DEBUG, "Shake one time to switch back");
        #endif
        current_state->handler();
        return;
      }
      #if defined (DEBUG)
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Shakes+1");
      #endif
      shakes = 1;
      app_timer_cancel(s_timeout_timer);
      app_timer_register(2500, prv_timer_shake_handler, NULL);
      break;
    case 1:
      #if defined (DEBUG)
        APP_LOG(APP_LOG_LEVEL_DEBUG, "SHAKE_SHAKE!!!!");
      #endif
      app_timer_cancel(s_timeout_timer);
      shakes = 0;
      current_state->handler();      
      break;
  }
}

static void accel_tap_handler(AccelAxisType axis, int32_t direction) {
   manage_shakes();
}

void init_states() {    
    prv_init_states();
    prv_set_init_state();
    accel_tap_service_subscribe(accel_tap_handler);    
}

void notify_settings_changed() {
    prv_set_init_state();
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
    reload_timer();    
  #if defined (DEBUG)
    APP_LOG(APP_LOG_LEVEL_DEBUG, "init handler %d", settings_get_PebbleShakeAction());
  #endif
  switch (settings_get_PebbleShakeAction()) {
    case PSA_NOTHING: 
        settings_set_ClockShowSeconds_disabled();
        current_state = &no_forecast;
        break;
    case PSA_FORECAST:
        settings_set_ClockShowSeconds_disabled();
        if (settings_get_WeatherStatus() == WEATHER_OK) {
            current_state = settings_get_ForecastEnabled() ? &clock_and_weather : &no_forecast;            
        } else {
            current_state = &no_forecast;
        }
        break;
    case PSA_SECONDS:
        settings_set_ClockShowSeconds_enabled();
        current_state = &seconds_clock;
        break;
  }
}
  
static void prv_init_timer(uint8_t swback_timeout) {
  if (swback_timeout > 0) {
    s_timeout_timer = app_timer_register(swback_timeout * 1000, prv_timer_timeout_handler, NULL);
  }  
}
  
static void prv_timer_timeout_handler(void *context) {  
  current_state->handler();
}

static void prv_timer_shake_handler(void *context) {
  #if defined (DEBUG)
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Double shake timeout");
  #endif
  shakes = 0;
}