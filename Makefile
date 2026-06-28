CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude
SRC = src/main.c src/execute.c src/nittalk.c
OBJ = $(SRC:.c=.o)
TARGET = octo-shell

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET)