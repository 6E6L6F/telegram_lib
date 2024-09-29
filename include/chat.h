#ifndef CHAT_H
#define CHAT_H

void sendMessageToGroup(const char *chat_id, const char *message);
void removeUserFromGroup(const char *chat_id, const char *user_id);
void listUsersInGroup(const char *chat_id);

#endif 
