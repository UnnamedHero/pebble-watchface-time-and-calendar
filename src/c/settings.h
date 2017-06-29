#pragma once

#define SETTINGS_KEY 1

typedef enum Update_Flag {
  UF_NOTHING,
  UF_WEATHER
} UPDATE_FLAG;

typedef void (*callback_ptr)(UPDATE_FLAG);

void init_settings(callback_ptr callback);
void populate_settings(DictionaryIterator *iter, void *context);
void save_settings();


char* settings_get_clockformat();
char* settings_get_weather_apikey();
