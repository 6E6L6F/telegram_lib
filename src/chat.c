#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "chat.h"
#include <json-c/json.h>
#include "telegram_lib.h"  

static int last_update_id = 0;    

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if(ptr == NULL) {
        printf("Not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

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

void add_admin(const char *chat_id, const char *user_id, int is_anonymous, int can_manage_chat, int can_post_messages, int can_edit_messages, int can_delete_messages, int can_manage_voice_chats) {
    char url[1024];
    char message[2048];

    snprintf(url, sizeof(url), TELEGRAM_API_URL "promoteChatMember", getBotToken());
    snprintf(message, sizeof(message),
             "{\"chat_id\": \"%s\", \"user_id\": \"%s\", \"is_anonymous\": %d, \"can_manage_chat\": %d, \"can_post_messages\": %d, \"can_edit_messages\": %d, \"can_delete_messages\": %d, \"can_manage_voice_chats\": %d}",
             chat_id, user_id, is_anonymous, can_manage_chat, can_post_messages, can_edit_messages, can_delete_messages, can_manage_voice_chats);

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

void remove_admin(const char *chat_id, const char *user_id) {
    char url[1024];
    char message[2048];

    snprintf(url, sizeof(url), TELEGRAM_API_URL "promoteChatMember", getBotToken());
    snprintf(message, sizeof(message),
             "{\"chat_id\": \"%s\", \"user_id\": \"%s\", \"can_manage_chat\": false, \"can_post_messages\": false, \"can_edit_messages\": false, \"can_delete_messages\": false, \"can_manage_voice_chats\": false}",
             chat_id, user_id);

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

void change_group_info(const char *chat_id, const char *new_title, const char *new_description) {
    char url[1024];
    char message[2048];

    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;
    curl = curl_easy_init();

    if (curl) {
        headers = curl_slist_append(headers, "Content-Type: application/json");

        if (new_title != NULL) {
            snprintf(url, sizeof(url), TELEGRAM_API_URL "setChatTitle", getBotToken());
            snprintf(message, sizeof(message), "{\"chat_id\": \"%s\", \"title\": \"%s\"}", chat_id, new_title);
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_URL, url);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, message);
            res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                fprintf(stderr, "Failed to set chat title: %s\n", curl_easy_strerror(res));
            }
        }

        if (new_description != NULL) {
            snprintf(url, sizeof(url), TELEGRAM_API_URL "setChatDescription", getBotToken());
            snprintf(message, sizeof(message), "{\"chat_id\": \"%s\", \"description\": \"%s\"}", chat_id, new_description);
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_URL, url);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, message);
            res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                fprintf(stderr, "Failed to set chat description: %s\n", curl_easy_strerror(res));
            }
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
}
char *parse_json_response_for_group_info(const char *json_data) {
    struct json_object *parsed_json;
    struct json_object *result;
    struct json_object *title;
    struct json_object *description;
    struct json_object *members_count;

    parsed_json = json_tokener_parse(json_data);

    json_object_object_get_ex(parsed_json, "result", &result);
    json_object_object_get_ex(result, "title", &title);
    json_object_object_get_ex(result, "description", &description);
    json_object_object_get_ex(result, "members_count", &members_count);

    char *group_info = (char *)malloc(1024);
    snprintf(group_info, 1024, "group_name: %s\ndescription: %s\nmembers_count: %d\n",
             json_object_get_string(title),
             json_object_get_string(description),
             json_object_get_int(members_count));

    json_object_put(parsed_json);  

    return group_info;
}

char* parse_json_response_for_invite_link(const char *response) {
    struct json_object *parsed_json;
    struct json_object *result;
    char *invite_link = NULL;

    parsed_json = json_tokener_parse(response);
        if (parsed_json == NULL) {
        return NULL;
    }
    if (json_object_object_get_ex(parsed_json, "result", &result)) {
        struct json_object *invite_link_obj;
        if (json_object_object_get_ex(result, "invite_link", &invite_link_obj)) {
            invite_link = strdup(json_object_get_string(invite_link_obj)); // کپی لینک دعوت
        }
    }

    json_object_put(parsed_json);
    return invite_link;
}

char *get_group_invite_link(const char *chat_id) {
    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;
    struct MemoryStruct chunk;

    chunk.memory = malloc(1);  
    chunk.size = 0;           

    curl = curl_easy_init();
    if(curl) {
        char url[1024];
        snprintf(url, sizeof(url), TELEGRAM_API_URL "exportChatInviteLink?chat_id=%s", getBotToken(), chat_id);

        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            free(chunk.memory);
            return NULL;
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);

        char *invite_link = NULL;
        invite_link = parse_json_response_for_invite_link(chunk.memory);  
        free(chunk.memory);
        
        return invite_link;  
    }

    return NULL;  
}


char *get_group_info(const char *chat_id) {
    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;
    struct MemoryStruct chunk;

    chunk.memory = malloc(1); 
    chunk.size = 0;           

    curl = curl_easy_init();
    if(curl) {
        char url[1024];
        snprintf(url, sizeof(url), TELEGRAM_API_URL "getChat?chat_id=%s", getBotToken(), chat_id);

        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            free(chunk.memory);
            return NULL;
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);

        char *group_info = NULL;
        group_info = parse_json_response_for_group_info(chunk.memory);  
        free(chunk.memory);

        return group_info;  
    }

    return NULL;  
}
int set_admin_title(const char *chat_id, const char *user_id, const char *custom_title) {
    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;
    struct MemoryStruct chunk;

    chunk.memory = malloc(1);
    chunk.size = 0;

    curl = curl_easy_init();
    if(curl) {
        char url[1024];
        snprintf(url, sizeof(url), TELEGRAM_API_URL "promoteChatMember?chat_id=%s&user_id=%s&custom_title=%s", getBotToken(), chat_id, user_id, custom_title);

        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            free(chunk.memory);
            return -1;
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
        free(chunk.memory);

        return 0;  
    }

    return -1;  
}

