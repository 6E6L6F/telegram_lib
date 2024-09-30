#ifndef CHAT_H
#define CHAT_H

#include <stdlib.h>

struct sender_info {
    int id;
    char first_name[64];
    char last_name[64];
    char username[64];
};

struct message_info {
    int message_id;
    char text[1024];
    struct sender_info sender;
    int date;
};

struct response_data {
    char *data;
    size_t size;
};

int get_updates(struct message_info **messages);
void sendMessageToGroup(const char *chat_id, const char *message);
void removeUserFromGroup(const char *chat_id, const char *user_id);
void listUsersInGroup(const char *chat_id);

#endif 
