#pragma once

#define SETTINGS_KEY 1
typedef void (*callback_ptr)();

void init_settings(callback_ptr callback);
void save_settings();
char* settings_get_clockformat();
