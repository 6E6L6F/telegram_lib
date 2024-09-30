#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "message.h"
#include "telegram_lib.h"  
#include <json-c/json.h>

void send_message(const char *chat_id, const char *message) {
    CURL *curl;
    CURLcode res;
    char url[512];
    char data[1024];

    snprintf(url, sizeof(url), TELEGRAM_API_URL "sendMessage", getBotToken());  
    snprintf(data, sizeof(data), "chat_id=%s&text=%s", chat_id, message);

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}

void send_photo(const char *chat_id, const char *photo_path) {
    CURL *curl;
    CURLcode res;
    char url[512];

    snprintf(url, sizeof(url), TELEGRAM_API_URL "sendPhoto", getBotToken());  

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        struct curl_httppost *formpost = NULL;
        struct curl_httppost *lastptr = NULL;

        curl_formadd(&formpost,
                     &lastptr,
                     CURLFORM_COPYNAME, "chat_id",
                     CURLFORM_COPYCONTENTS, chat_id,
                     CURLFORM_END);

        curl_formadd(&formpost,
                     &lastptr,
                     CURLFORM_COPYNAME, "photo",
                     CURLFORM_FILE, photo_path,
                     CURLFORM_END);

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
        curl_formfree(formpost);
    }

    curl_global_cleanup();
}

void send_video(const char *chat_id, const char *video_path) {
    CURL *curl;
    CURLcode res;
    char url[512];

    snprintf(url, sizeof(url), TELEGRAM_API_URL "sendVideo", getBotToken());

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        struct curl_httppost *formpost = NULL;
        struct curl_httppost *lastptr = NULL;

        curl_formadd(&formpost,
                     &lastptr,
                     CURLFORM_COPYNAME, "chat_id",
                     CURLFORM_COPYCONTENTS, chat_id,
                     CURLFORM_END);

        curl_formadd(&formpost,
                     &lastptr,
                     CURLFORM_COPYNAME, "video",
                     CURLFORM_FILE, video_path,
                     CURLFORM_END);

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
        curl_formfree(formpost);
    }

    curl_global_cleanup();
}

void send_audio(const char *chat_id, const char *audio_path) {
    CURL *curl;
    CURLcode res;
    char url[512];

    snprintf(url, sizeof(url), TELEGRAM_API_URL "sendAudio", getBotToken()); 

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        struct curl_httppost *formpost = NULL;
        struct curl_httppost *lastptr = NULL;

        curl_formadd(&formpost,
                     &lastptr,
                     CURLFORM_COPYNAME, "chat_id",
                     CURLFORM_COPYCONTENTS, chat_id,
                     CURLFORM_END);

        curl_formadd(&formpost,
                     &lastptr,
                     CURLFORM_COPYNAME, "audio",
                     CURLFORM_FILE, audio_path,
                     CURLFORM_END);

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

        res = curl_easy_perform(curl);

        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
        curl_formfree(formpost);
    }

    curl_global_cleanup();
}

void send_document(const char *chat_id, const char *document_path) {
    char url[512];
    snprintf(url, sizeof(url), TELEGRAM_API_URL "sendDocument", getBotToken());

    CURL *curl = curl_easy_init();
    if (curl) {
        struct curl_httppost *formpost = NULL;
        struct curl_httppost *lastptr = NULL;
        curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "chat_id", CURLFORM_COPYCONTENTS, chat_id, CURLFORM_END);
        curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "document", CURLFORM_FILE, document_path, CURLFORM_END);
        
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
        curl_easy_perform(curl);

        curl_easy_cleanup(curl);
        curl_formfree(formpost);
    }
}

void send_animation(const char *chat_id, const char *animation_path) {
    char url[512];
    snprintf(url, sizeof(url), TELEGRAM_API_URL "sendAnimation", getBotToken());

    CURL *curl = curl_easy_init();
    if (curl) {
        struct curl_httppost *formpost = NULL;
        struct curl_httppost *lastptr = NULL;
        curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "chat_id", CURLFORM_COPYCONTENTS, chat_id, CURLFORM_END);
        curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "animation", CURLFORM_FILE, animation_path, CURLFORM_END);
        
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
        curl_easy_perform(curl);

        curl_easy_cleanup(curl);
        curl_formfree(formpost);
    }
}

void send_voice(const char *chat_id, const char *voice_path) {
    char url[512];
    snprintf(url, sizeof(url), TELEGRAM_API_URL "sendVoice", getBotToken());
    CURL *curl = curl_easy_init();
    if (curl) {
        struct curl_httppost *formpost = NULL;
        struct curl_httppost *lastptr = NULL;
        curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "chat_id", CURLFORM_COPYCONTENTS, chat_id, CURLFORM_END);
        curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "voice", CURLFORM_FILE, voice_path, CURLFORM_END);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        curl_formfree(formpost);
    }
}

void send_location(const char *chat_id, double latitude, double longitude) {
    char url[512];
    snprintf(url, sizeof(url), TELEGRAM_API_URL "sendLocation?chat_id=%s&latitude=%f&longitude=%f", getBotToken(), chat_id, latitude, longitude);
    send_request(url);
}

void send_contact(const char *chat_id, const char *phone_number, const char *first_name) {
    char url[512];
    snprintf(url, sizeof(url), TELEGRAM_API_URL "sendContact?chat_id=%s&phone_number=%s&first_name=%s", getBotToken(), chat_id, phone_number, first_name);
    send_request(url);
}

void send_sticker(const char *chat_id, const char *sticker_file_id) {
    char url[512];
    snprintf(url, sizeof(url), TELEGRAM_API_URL "sendSticker?chat_id=%s&sticker=%s", getBotToken(), chat_id, sticker_file_id);
    send_request(url);
}

