#include <pebble.h>
#include "include/weather_full_m.h"
#include "include/weather_m.h"
#include "../settings.h"
#include "../3rdparty/locale_framework/localize.h"

static Layer *s_weather_full_layer;
static GFont s_wfont, s_wfont_sm;
static void prv_populate_weather_full_layer(Layer *, GContext *);

static const int descline_height = 20;
static const int icon_height = 24;
static const int text_height = 24;

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
	const int item_width = bb.size.w / 5;

	settings_get_theme(ctx);	
	
	GRect descline_rect = GRect (1, 0, bb.size.w, descline_height);	
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

	const int wind_x = 0;
	const int wind_y = descline_height + 5;
	const int wind_txt_x = 0;
	const int wind_txt_y = wind_y + text_height;

	GRect wind_rect = GRect (wind_x, wind_y, item_width, icon_height);	
	GRect wind_sp_rect = GRect (wind_txt_x, wind_txt_y, item_width -5,  text_height);

	graphics_draw_text(ctx, get_WeatherWindDirection(), \
	    s_wfont, \
    	wind_rect, \
    	GTextOverflowModeWordWrap, \
    	GTextAlignmentCenter, \
    	NULL);

	static char wind_speed[8];
	snprintf(wind_speed, sizeof(wind_speed), "%d", get_WeatherWindSpeed());
	graphics_draw_text(ctx, wind_speed, \
	    fonts_get_system_font(FONT_KEY_GOTHIC_24), \
    	wind_sp_rect, \
    	GTextOverflowModeWordWrap, \
    	GTextAlignmentCenter, \
    	NULL);

	const int press_x = item_width;
	const int press_y = wind_y;
	const int press_txt_x = press_x - 5;
	const int press_txt_y = wind_txt_y;

	GRect pressure_rect = GRect (press_x, press_y, item_width , icon_height);	
	GRect pressure_text_rect = GRect (press_txt_x, press_txt_y, item_width,  text_height);	
	graphics_draw_text(ctx, "w", \
	    s_wfont, \
    	pressure_rect, \
    	GTextOverflowModeWordWrap, \
    	GTextAlignmentCenter, \
    	NULL);

	static char pressure_text[8];
	snprintf(pressure_text, sizeof(pressure_text), "%d", get_WeatherPressure());
	graphics_draw_text(ctx, pressure_text, \
	    fonts_get_system_font(FONT_KEY_GOTHIC_24), \
    	pressure_text_rect, \
    	GTextOverflowModeWordWrap, \
    	GTextAlignmentCenter, \
    	NULL);


	const int hum_x = item_width * 2;
	const int hum_y = wind_y;
	const int hum_txt_x = hum_x;
	const int hum_txt_y = wind_txt_y;

	GRect hum_rect = GRect (hum_x, hum_y, item_width , icon_height);	
	GRect hum_text_rect = GRect (hum_txt_x, hum_txt_y, item_width, text_height);	
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