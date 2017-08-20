#pragma once

void init_message_system();
void deinit_message_system();

typedef void (*message_failed_callback_ptr)(void);

void send_message(Tuplet*, int data_array_size, message_failed_callback_ptr);