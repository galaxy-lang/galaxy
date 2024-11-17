TARGET = main
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

CC = clang
CFLAGS = -Wall -Wint-conversion -Wextra -std=c11 -O2

SRC_FILES = $(shell find $(SRC_DIR) -type f -name '*.c')
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

all: $(BIN_DIR)/$(TARGET)

$(BIN_DIR)/$(TARGET): $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^
	rm -rf $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

rebuild: clean all

lint:
	@echo "Running clang-tidy on all source files..."
	@$(foreach file, $(SRC_FILES), clang-tidy $(file) -- -std=c11 || exit 1;)
