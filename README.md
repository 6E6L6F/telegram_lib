# Telegram Bot Library

This project provides a C library for interacting with Telegram bots. It implements various features such as sending messages, handling updates, and more.

## Features

- Set and get the bot token
- Send messages (text, photos, videos, audio)
- Handle updates from the Telegram API
- Support for using libcurl and json-c for HTTP requests and JSON parsing

## Requirements

- GCC (GNU Compiler Collection)
- libcurl
- json-c

## Installation

1. Clone the repository:

   ```bash
   git clone https://github.com/6E6L6F/telegram_lib
   cd telegram_lib
   ```

2. Compile the project:

   ```bash
   make
   ```

3. Run the bot:

   ```bash
   ./telegram_bot
   ```

## Usage

### Setting the Bot Token

To set the bot token, use the `setBotToken` function:

```c
setBotToken("YOUR_BOT_TOKEN");
```

### Sending a Message

To send a message, use the `send_message` function:

```c
send_message("CHAT_ID", "MESSAGE");
```

### Getting Updates

To get updates from Telegram, call the `get_updates` function:

```c
get_updates();
```

### Sending a Photo

To send photo from Telegram, call the `send_photo` function:

```c
send_photo("CHAT_ID","PATH");
```

### Sending a Video

To send video from Telegram, call the `send_video` function:

```c
send_video("CHAT_ID","PATH");
```

### Sending a Audio

To send audio from Telegram, call the `send_audio` function:

```c
send_audio("CHAT_ID","PATH");
```

### Sending a Message To Group

To send message to group from Telegram, call the `sendMessageToGroup` function:

```c
sendMessageToGroup("CHAT_ID","MESSAGE");
```

### Removeing User From Group

To remove user from group from Telegram, call the `removeUserFromGroup` function:

```c
removeUserFromGroup("CHAT_ID","USER_ID");
```

### List Users In Group

To list users in group from Telegram, call the `listUsersInGroup` function:

```c
listUsersInGroup("CHAT_ID");
```


### User Account Information 

To get user account information from Telegram, call the `getUserInfo` function:

```c
getUserInfo("USER_ID");
```
