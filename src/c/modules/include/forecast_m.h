#pragma once

#define FORECAST_KEY 3

void init_forecast_layer(GRect);
void deinit_forecast_layer();
Layer* get_layer_forecast();
void update_forecast(bool);
void forecast_update(DictionaryIterator *iter, void *context);