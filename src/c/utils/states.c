#include <pebble.h>
#include "include/states.h"
#include "../settings.h"
#include "include/ticktimerhelper.h"
#include "../windows/include/time-window.h"
#include "../windows/include/forecast-window.h"


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
static void prv_init_states();

static void accel_tap_handler(AccelAxisType axis, int32_t direction) { 
    current_state->handler();
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
  //reload_timer();
}
  
static void cw_f_accel_handler() {
  #if defined (DEBUG)
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Handling cw_f_accel_handler");
  #endif  
  window_stack_push(get_time_window(), true);
  app_timer_cancel(s_timeout_timer);
  prv_set_init_state();
  //reload_timer();
}
  
static void cw_no_forecast_handler() {
  #if defined (DEBUG)
    APP_LOG(APP_LOG_LEVEL_DEBUG, "cw_no_forecast handler");
  #endif
  //reload_timer();
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
  
static void prv_timer_timeout_handler (void *context) {  
  current_state->handler();
}