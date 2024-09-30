#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"
#include "telegram_lib.h"  
#include <json-c/json.h>


struct json_object* get_user_info(const char *chat_id, const char *user_id) {
    char url[256];
    snprintf(url, sizeof(url), TELEGRAM_API_URL "getChatMember?chat_id=%s&user_id=%s", getBotToken(), chat_id, user_id);
    char *response = send_request(url);
    struct json_object *parsed_json = json_tokener_parse(response);
    free(response);
    return parsed_json;
}


void send_chat_action(const char *chat_id, const char *action) {
    char url[256];
    snprintf(url, sizeof(url), TELEGRAM_API_URL "sendChatAction?chat_id=%s&action=%s", getBotToken(), chat_id, action);
    send_request(url);
}


void send_invoice(const char *chat_id, const char *title, const char *description, const char *payload, const char *provider_token, const char *currency, int price) {
    char url[1024];
    snprintf(url, sizeof(url),
            TELEGRAM_API_URL "sendInvoice?chat_id=%s&title=%s&description=%s&payload=%s&provider_token=%s&currency=%s&prices[0][label]=Product&prices[0][amount]=%d",
            getBotToken(), chat_id, title, description, payload, provider_token, currency, price);
    send_request(url);
}