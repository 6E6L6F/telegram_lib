#ifndef MESSAGE_H
#define MESSAGE_H

#include "telegram_lib.h"

void send_message(const char *chat_id, const char *message);
void send_photo(const char *chat_id, const char *photo_path);
void send_video(const char *chat_id, const char *video_path);
void send_audio(const char *chat_id, const char *audio_path);
void send_document(const char *chat_id, const char *document_path);
void send_animation(const char *chat_id, const char *animation_path);
void send_video(const char *chat_id, const char *video_path);
void send_voice(const char *chat_id, const char *voice_path);
void send_location(const char *chat_id, double latitude, double longitude);
void send_contact(const char *chat_id, const char *phone_number, const char *first_name);
void send_sticker(const char *chat_id, const char *sticker_file_id);

#endif 
