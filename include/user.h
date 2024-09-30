#ifndef USER_H
#define USER_H
#include <stdio.h>
#include <json-c/json.h>


struct json_object* get_user_info(const char *chat_id, const char *user_id);
void send_private_message(const char *user_id, const char *message);
void send_chat_action(const char *chat_id, const char *action);
void send_invoice(const char *chat_id, const char *title, const char *description, const char *payload, const char *provider_token, const char *currency, int price);

#endif 
