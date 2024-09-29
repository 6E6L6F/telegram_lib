#ifndef MESSAGE_H
#define MESSAGE_H

#include "telegram_lib.h"

void get_updates(void);
void send_message(const char *chat_id, const char *message);
void send_photo(const char *chat_id, const char *photo_path);
void send_video(const char *chat_id, const char *video_path);
void send_audio(const char *chat_id, const char *audio_path);

#endif 
