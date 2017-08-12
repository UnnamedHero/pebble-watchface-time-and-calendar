#pragma once

void init_time_window();
void deinit_time_window();
Window* get_time_window();

void window_update_time(struct tm *);
void ready_for_weather(bool);
void simple_weather_update(DictionaryIterator *iter, void *context);
void time_window_force_redraw();