#ifndef TELEGRAM_LIB_H
#define TELEGRAM_LIB_H

#include "chat.h"
#include "message.h"
#include "user.h"
#include "buttons.h"

#define TELEGRAM_API_URL "https://api.telegram.org/bot%s/"

char* send_request(const char *url);
void setBotToken(const char *token);
const char *getBotToken(void);



#endif 
