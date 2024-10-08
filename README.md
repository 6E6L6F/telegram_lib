
# Telegram Bot Library

This project provides a C library for interacting with Telegram bots. It implements various features such as sending messages, handling updates, and more.

## Features

- Set and get the bot token
- Send messages (text, photos, videos, audio)
- Handle updates from the Telegram API
- Support for using libcurl and json-c for HTTP requests and JSON parsing
- Create inline and reply keyboards
- Request user location and contact information

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

To send a photo from Telegram, call the `send_photo` function:

```c
send_photo("CHAT_ID", "PATH");
```

### Sending a Video

To send a video from Telegram, call the `send_video` function:

```c
send_video("CHAT_ID", "PATH");
```

### Sending Audio

To send audio from Telegram, call the `send_audio` function:

```c
send_audio("CHAT_ID", "PATH");
```


### Removing User from Group

To remove a user from a group in Telegram, call the `removeUserFromGroup` function:

```c
removeUserFromGroup("CHAT_ID", "USER_ID");
```

### Listing Users in Group

To list users in a group from Telegram, call the `listUsersInGroup` function:

```c
listUsersInGroup("CHAT_ID");
```

### User Account Information

To get user account information from Telegram, call the `getUserInfo` function:

```c
getUserInfo("USER_ID");
```

### Sending Inline Keyboard

To send a message with an inline keyboard, use the `send_inline_keyboard` function:

```c
send_inline_keyboard("CHAT_ID", "Please select an option:", "callback_data_example");
```

### Sending Reply Keyboard

To send a message with a reply keyboard, use the `send_reply_keyboard` function:

```c
send_reply_keyboard("CHAT_ID", "Please select one of the options:", "Option 1");
```

### Removing Keyboard

To remove the keyboard from the chat and send a message, use the `remove_keyboard` function:

```c
remove_keyboard("CHAT_ID", "Keyboard removed.");
```

### Sending Inline Keyboard with Multiple Buttons

To send a message with multiple inline keyboard buttons, use the `send_inline_keyboard_multiple` function:

```c
const char *buttons[] = {"Button 1", "Button 2", "Button 3"};
send_inline_keyboard_multiple("CHAT_ID", "Choose an option:", buttons, 3);
```

### Creating Location Request Button

To create a button that requests the user's location, use the `create_location_request_button` function:

```c
create_location_request_button("CHAT_ID" ,"Send your location");
```

### Creating Contact Request Button

To create a button that requests the user's contact information, use the `create_contact_request_button` function:

```c
create_contact_request_button("CHAT_ID" ,"Send your contact number");
```


### Adding Admin

To add an admin to a group, use the add_admin function:

```c
add_admin("CHAT_ID", "USER_ID");
```

### Removing Admin

To remove an admin from a group, use the remove_admin function:

```c
remove_admin("CHAT_ID", "USER_ID");
```

### Changing Group Information

To change the group profile picture, name, or description, use the change_group_info function:

```c
change_group_info("CHAT_ID", "New Group Name", "New Description", "PATH_TO_NEW_PHOTO");
```

### Getting Group Link

To get the group's invite link, use the get_group_invite_link function:


```c
char *link = get_group_invite_link("CHAT_ID");
```

### Getting Group Information

To get the information of a group, use the get_group_info function:


```c
get_group_info("CHAT_ID");
```








## Conclusion

This library simplifies interactions with the Telegram Bot API, making it easier to build interactive bots. Customize the functions as needed for your specific requirements.
