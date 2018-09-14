#pragma once

#define WEATHER_KEY 4

void init_weather_layer(GRect);
void deinit_weather_layer();
void update_weather(bool);
void get_weather(DictionaryIterator *, void *);
Layer* get_layer_weather();

 uint8_t get_weatherIsReady();
 int get_WeatherTemperature();
 char* get_WeatherCondition();
 char* get_WeatherDesc();
 uint32_t get_WeatherTimeStamp();
 int get_WeatherPressure();
 uint8_t get_WeatherWindSpeed();
 char* get_WeatherWindDirection();
 uint8_t get_WeatherHumidity();
 char* get_WeatherSunset();
 char* get_WeatherSunrise();
