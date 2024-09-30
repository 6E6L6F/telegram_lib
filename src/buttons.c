#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include "buttons.h"
#include "telegram_lib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "buttons.h"

void send_inline_keyboard(const char *chat_id, const char *text, const char *callback_data) {
    char url[1024];
    char message[2048];

    snprintf(url, sizeof(url), TELEGRAM_API_URL "sendMessage", getBotToken());
    snprintf(message, sizeof(message),
             "{\"chat_id\": \"%s\", \"text\": \"%s\", \"reply_markup\": {\"inline_keyboard\": [[{\"text\": \"%s\", \"callback_data\": \"%s\"}]]}}",
             chat_id, text, text, callback_data);

    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;
    curl = curl_easy_init();
    if (curl) {
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, message);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
}

void send_reply_keyboard(const char *chat_id, const char *text, const char *button_text) {
    char url[1024];
    char message[2048];

    snprintf(url, sizeof(url), TELEGRAM_API_URL "sendMessage", getBotToken());
    snprintf(message, sizeof(message),
             "{\"chat_id\": \"%s\", \"text\": \"%s\", \"reply_markup\": {\"keyboard\": [[{\"text\": \"%s\"}]], \"resize_keyboard\": true}}",
             chat_id, text, button_text);

    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;
    curl = curl_easy_init();
    if (curl) {
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, message);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
}

void remove_keyboard(const char *chat_id, const char *text) {
    char url[1024];
    char message[1024];

    snprintf(url, sizeof(url), TELEGRAM_API_URL "sendMessage", getBotToken());
    snprintf(message, sizeof(message),
             "{\"chat_id\": \"%s\", \"text\": \"%s\", \"reply_markup\": {\"remove_keyboard\": true}}",
             chat_id, text);

    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;
    curl = curl_easy_init();
    if (curl) {
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, message);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
}

void send_inline_keyboard_multiple(const char *chat_id, const char *text, const char *callback_data[], size_t button_count) {
    char url[1024];
    char message[4096];
    char buttons[2048] = "";

    for (size_t i = 0; i < button_count; i++) {
        char button[512];
        snprintf(button, sizeof(button), "{\"text\": \"%s\", \"callback_data\": \"%s\"}", callback_data[i], callback_data[i]);
        strcat(buttons, button);
        if (i < button_count - 1) {
            strcat(buttons, ",");
        }
    }

    snprintf(url, sizeof(url), TELEGRAM_API_URL "sendMessage", getBotToken());
    snprintf(message, sizeof(message),
             "{\"chat_id\": \"%s\", \"text\": \"%s\", \"reply_markup\": {\"inline_keyboard\": [[%s]]}}",
             chat_id, text, buttons);

    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;
    curl = curl_easy_init();
    if (curl) {
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, message);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
}


void create_location_request_button(const char *chat_id ,const char *text) {
    char url[1024];
    snprintf(url, sizeof(url), TELEGRAM_API_URL "sendMessage?chat_id=%s&text=%s&reply_markup={\"keyboard\":[[{\"text\":\"%s\",\"request_location\":true}]],\"resize_keyboard\":true}", getBotToken(),chat_id ,text, text);

    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        curl_easy_cleanup(curl);
    }
}

void create_contact_request_button(const char *chat_id , const char *text) {
    char url[1024];
    snprintf(url, sizeof(url), TELEGRAM_API_URL "sendMessage?chat_id=%s&text=%s&reply_markup={\"keyboard\":[[{\"text\":\"%s\",\"request_contact\":true}]],\"resize_keyboard\":true}", getBotToken(), chat_id ,text, text);

    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        curl_easy_cleanup(curl);
    }
}