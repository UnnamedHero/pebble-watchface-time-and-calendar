#pragma once

void init_forecast_window();
void deinit_forecast_window();
Window* get_forecast_window();

// void window_update_forecast(struct tm *);
void ready_for_forecast(bool);
void forecast_update_disp(DictionaryIterator *iter, void *context);
// void forecast_window_force_redraw();