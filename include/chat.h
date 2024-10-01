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

struct MemoryStruct {
    char *memory;
    size_t size;
};


int get_updates(struct message_info **messages);
void sendMessageToGroup(const char *chat_id, const char *message);
void removeUserFromGroup(const char *chat_id, const char *user_id);
void listUsersInGroup(const char *chat_id);
void add_admin(const char *chat_id, const char *user_id, int is_anonymous, int can_manage_chat, int can_post_messages, int can_edit_messages, int can_delete_messages, int can_manage_voice_chats);
void remove_admin(const char *chat_id, const char *user_id);
void change_group_info(const char *chat_id, const char *new_name, const char *new_description);
char* get_group_invite_link(const char *chat_id);
char* get_group_info(const char *chat_id);
int set_admin_title(const char *chat_id, const char *user_id, const char *custom_title);
char* parse_json_response_for_invite_link(const char *response);

#endif 
