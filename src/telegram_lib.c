#include "telegram_lib.h"
#include <stdlib.h>  
#include <stddef.h>  
#include <string.h> 

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
