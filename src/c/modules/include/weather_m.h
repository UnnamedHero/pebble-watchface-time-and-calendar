#pragma once

void init_weather_layer(GRect);
void deinit_weather_layer();
void update_weather();
void get_weather(DictionaryIterator *, void *);
Layer* get_layer_weather();