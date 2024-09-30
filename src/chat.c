#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "chat.h"
#include <json-c/json.h>
#include "telegram_lib.h"  

static int last_update_id = 0;            

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


int get_updates(struct message_info **messages) {
    char url[1024];
    snprintf(url, sizeof(url), TELEGRAM_API_URL "getUpdates?offset=%d", getBotToken(), last_update_id + 1);

    char *response = send_request(url);  
    if (!response) {
        printf("Error fetching updates.\n");
        return 0;
    }

    struct json_object *parsed_json = json_tokener_parse(response);
    struct json_object *result;
    
    if (!json_object_object_get_ex(parsed_json, "result", &result)) {
        printf("No result found.\n");
        json_object_put(parsed_json);
        return 0;
    }

    size_t num_messages = json_object_array_length(result);
    *messages = malloc(num_messages * sizeof(struct message_info));
    for (size_t i = 0; i < num_messages; i++) {
        json_object *message = json_object_array_get_idx(result, i);
        json_object *from_obj;

        json_object *message_id_obj, *text_obj;
        json_object_object_get_ex(message, "message_id", &message_id_obj);
        json_object_object_get_ex(message, "text", &text_obj);
        json_object_object_get_ex(message, "from", &from_obj);

        (*messages)[i].message_id = json_object_get_int(message_id_obj);
        strncpy((*messages)[i].text, json_object_get_string(text_obj), sizeof((*messages)[i].text));

        json_object *id_obj, *first_name_obj, *last_name_obj, *username_obj;
        json_object_object_get_ex(from_obj, "id", &id_obj);
        json_object_object_get_ex(from_obj, "first_name", &first_name_obj);
        json_object_object_get_ex(from_obj, "last_name", &last_name_obj);
        json_object_object_get_ex(from_obj, "username", &username_obj);

        (*messages)[i].sender.id = json_object_get_int(id_obj);
        strncpy((*messages)[i].sender.first_name, json_object_get_string(first_name_obj), sizeof((*messages)[i].sender.first_name));
        strncpy((*messages)[i].sender.last_name, last_name_obj ? json_object_get_string(last_name_obj) : "", sizeof((*messages)[i].sender.last_name));
        strncpy((*messages)[i].sender.username, username_obj ? json_object_get_string(username_obj) : "", sizeof((*messages)[i].sender.username));
        (*messages)[i].date = json_object_get_int(json_object_object_get(message, "date"));

        last_update_id = (*messages)[i].message_id;
    }

    json_object_put(parsed_json);  
    return num_messages;  
}

