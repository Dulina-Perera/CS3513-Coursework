CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = rpal_interpreter
SRC_DIR = src
BUILD_DIR = build

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean run

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(BUILD_DIR)/*.o $(TARGET)
