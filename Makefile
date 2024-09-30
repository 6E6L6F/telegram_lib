CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

OBJ = src/chat.o src/message.o src/main.o src/telegram_lib.o src/buttons.o src/user.o 

LIBS = -lcurl -ljson-c

TARGET = telegram_bot

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LIBS)

%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: clean all
