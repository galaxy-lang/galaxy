TARGET = main
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

CC = clang
CFLAGS = -Wall -Wint-conversion -Wextra -O2

# Arquivos principais (excluindo arquivos *.test.c)
SRC_FILES = $(shell find $(SRC_DIR) -type f -name '*.c' ! -name '*.test.c')
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

# Arquivos de teste (apenas arquivos *.test.c)
TEST_FILES = $(shell find $(SRC_DIR) -type f -name '*.test.c')
TEST_BINS = $(patsubst $(SRC_DIR)/%.test.c, $(BIN_DIR)/%, $(TEST_FILES))

.PHONY: all clean rebuild lint test

# Regra principal para compilar apenas o programa principal
all: $(BIN_DIR)/$(TARGET)

$(BIN_DIR)/$(TARGET): $(OBJ_FILES) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^
	rm -rf $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para compilar arquivos de teste (compila diretamente para bin sem hierarquia)
test: $(TEST_BINS)
	@echo "Compilation of test files completed."

# Regra para compilar cada arquivo de teste e gerar binários diretamente no diretório bin
$(BIN_DIR)/%: $(SRC_DIR)/%.test.c $(SRC_DIR)/lexer/lexer.c $(SRC_DIR)/lexer/lexer_error.c | $(BIN_DIR)
	@mkdir -p $(BIN_DIR)  # Cria bin caso não exista
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$(notdir $@) $< $(SRC_DIR)/lexer/lexer.c $(SRC_DIR)/lexer/lexer_error.c

# Garante a criação dos diretórios necessários
$(OBJ_DIR) $(BIN_DIR):
	@mkdir -p $@

# Regra para limpar arquivos gerados
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Regra para recompilar apenas o programa principal
rebuild: clean all

# Regra para analisar o código com clang-tidy
lint:
	@echo "Running clang-tidy on all source files..."
	@$(foreach file, $(SRC_FILES), clang-tidy $(file) -- -std=c11 || exit 1;)

# Mensagem caso não haja arquivos de teste
ifeq ($(TEST_FILES),)
test:
	@echo "No test files found in $(SRC_DIR)."
endif
