#pragma once

void init_bluetooh_layer(GRect);
void deinit_bluetooth_layer();

Layer* get_layer_bluetooth();

bool is_bt_connected();