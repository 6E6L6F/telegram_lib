CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
OBJ = src/chat.o src/message.o src/main.o src/telegram_lib.o

telegram_bot: $(OBJ)
	$(CC) -o $@ $^ -lcurl -ljson-c

%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) telegram_bot
