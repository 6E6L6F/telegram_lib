#ifndef TELEGRAM_BUTTONS_H
#define TELEGRAM_BUTTONS_H

void send_inline_keyboard(const char *chat_id, const char *text, const char *callback_data);
void send_reply_keyboard(const char *chat_id, const char *text, const char *button_text);
void remove_keyboard(const char *chat_id, const char *text);
void send_inline_keyboard_multiple(const char *chat_id, const char *text, const char *callback_data[], size_t button_count);
void create_location_request_button(const char *chat_id ,const char *text);
void create_contact_request_button(const char *chat_id ,const char *text);

#endif
