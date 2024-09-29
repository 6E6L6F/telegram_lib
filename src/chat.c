#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "chat.h"
#include "json-c/json.h"
#include "telegram_lib.h"  

static void sendPostRequest(const char *url, const char *json_data) {
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, (struct curl_slist *)curl_slist_append(NULL, "Content-Type: application/json"));
        
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

void sendMessageToGroup(const char *chat_id, const char *message) {
    char url[256];
    snprintf(url, sizeof(url), "%s/sendMessage", TELEGRAM_API_URL); 
    
    struct json_object *jobj = json_object_new_object();
    json_object_object_add(jobj, "chat_id", json_object_new_string(chat_id));
    json_object_object_add(jobj, "text", json_object_new_string(message));
    
    const char *json_str = json_object_to_json_string(jobj);
    sendPostRequest(url, json_str);
    
    json_object_put(jobj);
}


void removeUserFromGroup(const char *chat_id, const char *user_id) {
    char url[256];
    snprintf(url, sizeof(url), "%s/%s/removeChatMember", TELEGRAM_API_URL, chat_id); 
    
    struct json_object *jobj = json_object_new_object();
    json_object_object_add(jobj, "user_id", json_object_new_string(user_id));
    
    const char *json_str = json_object_to_json_string(jobj);
    sendPostRequest(url, json_str);
    
    json_object_put(jobj);
}

void listUsersInGroup(const char *chat_id) {
    char url[256];
    snprintf(url, sizeof(url), "%s/%s/getChatAdministrators", TELEGRAM_API_URL, chat_id); 
    
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}
