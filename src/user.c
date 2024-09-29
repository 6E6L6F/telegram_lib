
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include "telegram_lib.h"  
#include "user.h"



User *users = NULL;
size_t user_count = 0;
struct MemoryStruct {
    char *memory;
    size_t size;
};

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, struct MemoryStruct *userp) {
    size_t realsize = size * nmemb;
    userp->memory = realloc(userp->memory, userp->size + realsize + 1);
    if (userp->memory == NULL) {
        printf("Not enough memory\n");
        return 0;  
    }

    memcpy(&(userp->memory[userp->size]), contents, realsize);
    userp->size += realsize;
    userp->memory[userp->size] = 0;

    return realsize;
}

void getUserInfo(const char *chat_id) {
    CURL *curl;
    CURLcode res;

    struct MemoryStruct chunk;
    chunk.memory = malloc(1);  
    chunk.size = 0;             

    curl = curl_easy_init();
    if (curl) {
        char url[256];
        snprintf(url, sizeof(url), TELEGRAM_API_URL "getChat", getBotToken(), chat_id); 

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            printf("User Info: %s\n", chunk.memory);
        }

        curl_easy_cleanup(curl);
        free(chunk.memory);
    }
}

void addUser(const char *id, const char *first_name, const char *last_name, const char *username) {
    for (size_t i = 0; i < user_count; i++) {
        if (strcmp(users[i].id, id) == 0) {
            return;  
        }
    }

    users = realloc(users, sizeof(User) * (user_count + 1));
    users[user_count].id = strdup(id);
    users[user_count].firstName = strdup(first_name);
    users[user_count].lastName = strdup(last_name);
    users[user_count].username = strdup(username);
    user_count++;
}

void handleNewUser(const char *json_response) {
    struct json_object *parsed_json = json_tokener_parse(json_response);
    struct json_object *result;
    struct json_object *new_user;

    json_object_object_get_ex(parsed_json, "result", &result);
    json_object_object_get_ex(result, "new_chat_member", &new_user);

    const char *id = json_object_get_string(json_object_object_get(new_user, "id"));
    const char *first_name = json_object_get_string(json_object_object_get(new_user, "first_name"));
    const char *last_name = json_object_get_string(json_object_object_get(new_user, "last_name"));
    const char *username = json_object_get_string(json_object_object_get(new_user, "username"));

    addUser(id, first_name, last_name, username);

    json_object_put(parsed_json); 
    }
