#include <pebble.h>
#include "include/weather_full_m.h"
#include "include/weather_m.h"
#include "../settings.h"
#include "../3rdparty/locale_framework/localize.h"

static Layer *s_weather_full_layer;
static GFont s_wfont, s_wfont_sm;
static void prv_populate_weather_full_layer(Layer *, GContext *);


void init_weather_full_layer(GRect rect) {
  s_weather_full_layer = layer_create(rect);
  //s_weather_full_layer = layer;
  layer_set_update_proc(s_weather_full_layer, prv_populate_weather_full_layer);
  s_wfont = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_CLIMACONS_42));
  s_wfont_sm = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_CLIMACONS_36));  
}

void deinit_weather_full_layer() {
	layer_destroy(s_weather_full_layer);
	fonts_unload_custom_font(s_wfont);
	fonts_unload_custom_font(s_wfont_sm);
}

Layer* get_layer_weather_full() {
	return s_weather_full_layer;
}

static void prv_populate_weather_full_layer(Layer *me, GContext *ctx) {

	if (get_weatherIsReady() != 1) {
		return;
	}	
	GRect bb = layer_get_bounds(me);
	GRect descline_rect = GRect (2, 0, bb.size.w, 20);	
	static char descline[64];
	snprintf(descline, sizeof(descline), "%d°, %s",\
		get_WeatherTemperature(),\
		get_WeatherDesc());
	graphics_draw_text(ctx, descline, \
	    fonts_get_system_font(FONT_KEY_GOTHIC_24), \
    	descline_rect, \
    	GTextOverflowModeWordWrap, \
    	GTextAlignmentLeft, \
    	NULL);        

	GRect wind_rect = GRect (1, 26, bb.size.w / 4 , 20);	
	GRect wind_sp_rect = GRect (3, 48, bb.size.w / 4,  24);	
	graphics_draw_text(ctx, get_WeatherWindDirection(), \
	    s_wfont, \
    	wind_rect, \
    	GTextOverflowModeWordWrap, \
    	GTextAlignmentLeft, \
    	NULL);

	static char wind_speed[8];
	snprintf(wind_speed, sizeof(wind_speed), "%d", get_WeatherWindSpeed());
	graphics_draw_text(ctx, wind_speed, \
	    fonts_get_system_font(FONT_KEY_GOTHIC_24), \
    	wind_sp_rect, \
    	GTextOverflowModeWordWrap, \
    	GTextAlignmentLeft, \
    	NULL);

	GRect pressure_rect = GRect (25, 32, bb.size.w / 4 , 20);	
	GRect pressure_text_rect = GRect (22, 48, bb.size.w / 4,  24);	
	graphics_draw_text(ctx, "w", \
	    s_wfont, \
    	pressure_rect, \
    	GTextOverflowModeWordWrap, \
    	GTextAlignmentLeft, \
    	NULL);

	static char pressure_text[8];
	snprintf(pressure_text, sizeof(pressure_text), "%d", get_WeatherPressure());
	graphics_draw_text(ctx, pressure_text, \
	    fonts_get_system_font(FONT_KEY_GOTHIC_24), \
    	pressure_text_rect, \
    	GTextOverflowModeWordWrap, \
    	GTextAlignmentLeft, \
    	NULL);

	GRect hum_rect = GRect (55, 30, bb.size.w / 4 , 20);	
	GRect hum_text_rect = GRect (58, 48, bb.size.w / 4,  24);	
	graphics_draw_text(ctx, "x", \
	    s_wfont, \
    	hum_rect, \
    	GTextOverflowModeWordWrap, \
    	GTextAlignmentLeft, \
    	NULL);

	static char hum_text[8];
	snprintf(hum_text, sizeof(hum_text), "%d", get_WeatherHumidity());
	graphics_draw_text(ctx, hum_text, \
	    fonts_get_system_font(FONT_KEY_GOTHIC_24), \
    	hum_text_rect, \
    	GTextOverflowModeWordWrap, \
    	GTextAlignmentLeft, \
    	NULL);

	GRect sunrise_rect = GRect (80, 19, bb.size.w / 4 , 20);
	GRect sunset_rect = GRect (80, 42, bb.size.w / 4 , 20);	
	GRect sunrise_text_rect = GRect (110, 28, bb.size.w / 3,  24);	
	GRect sunset_text_rect = GRect (110, 52, bb.size.w / 3,  24);	
	graphics_draw_text(ctx, "K", \
	    s_wfont_sm, \
    	sunrise_rect, \
    	GTextOverflowModeWordWrap, \
    	GTextAlignmentLeft, \
    	NULL);
	graphics_draw_text(ctx, "J", \
	    s_wfont_sm, \
    	sunset_rect, \
    	GTextOverflowModeWordWrap, \
    	GTextAlignmentLeft, \
    	NULL);

	// static char sunrise_text[8];
	// snprintf(sunrise_text, sizeof(sunrise_text), "%s\n%s", get_WeatherSunrise(), get_WeatherSunset());
	graphics_draw_text(ctx, get_WeatherSunrise(), \
	    fonts_get_system_font(FONT_KEY_GOTHIC_18), \
    	sunrise_text_rect, \
    	GTextOverflowModeWordWrap, \
    	GTextAlignmentLeft, \
    	NULL);

	// static char sunset_text[8];
	// snprintf(sunset_text, sizeof(sunset_text), "%s\n%s", get_WeatherSunrise(), get_WeatherSunset());
	graphics_draw_text(ctx, get_WeatherSunset(), \
	    fonts_get_system_font(FONT_KEY_GOTHIC_18), \
    	sunset_text_rect, \
    	GTextOverflowModeWordWrap, \
    	GTextAlignmentLeft, \
    	NULL);
}