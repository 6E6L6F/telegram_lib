#include "telegram_lib.h"
#include <stdlib.h>  
#include <stddef.h>  
#include <string.h> 
#include <curl/curl.h>

static char *botToken = NULL;  

void setBotToken(const char *token) {
    if (botToken) {
        free(botToken);  
    }
    botToken = strdup(token);  
}

const char *getBotToken(void) {
    return botToken;  
}

char* send_request(const char *url) {
    CURL *curl;
    char *response = malloc(2048);  
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);  
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return response;
}

