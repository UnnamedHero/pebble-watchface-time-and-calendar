#pragma once

void init_time_window();
void deinit_time_window();
Window* get_time_window();

void update_time();
void simple_weather_update(DictionaryIterator *iter, void *context);
