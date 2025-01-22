# Компилятор и флаги
CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
LDFLAGS = -lncurses -lcheck -lm -lpthread -lsubunit


# Папки
OBJ_DIR = ../build
SRC_DIR = .
BRICK_GAME_DIR = $(SRC_DIR)/brick_game/tetris
GUI_CLI_DIR = $(SRC_DIR)/gui/cli
TEST_DIR = $(SRC_DIR)/tests

# Цели
EXE = ../tetris
LIB = ../libtetris.a
TEST_EXE = ../tetris_test

# Исходные и объектные файлы для основной программы
SRC_FILES = $(wildcard $(BRICK_GAME_DIR)/*.c) $(wildcard $(GUI_CLI_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

# Исходные файлы для тестов (только для brick_game/tetris)
TEST_SRC = $(TEST_DIR)/test.c
TEST_OBJ = $(OBJ_DIR)/test.o
TEST_SRC_FILES = $(wildcard $(BRICK_GAME_DIR)/*.c)
TEST_OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(TEST_SRC_FILES))

# Основная цель
all: clean $(EXE)

# Сборка исполняемого файла
$(EXE): $(OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Правило для объектных файлов
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)/$(dir $<)  # Создаём папки для объектных файлов в build
	$(CC) $(CFLAGS) -c $< -o $@

# Создание библиотеки
lib: $(LIB)

$(LIB): $(OBJ_FILES)
	ar rc $@ $^
	ranlib $@

# Цель для тестов
test: $(TEST_EXE)
	./$(TEST_EXE)

# Сборка исполняемого файла для тестов (только для brick_game/tetris)
$(TEST_EXE): $(TEST_OBJ) $(TEST_OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Компиляция файла с тестами
$(TEST_OBJ): $(TEST_SRC)
	@mkdir -p $(OBJ_DIR)/$(dir $<)
	$(CC) $(CFLAGS) -c $< -o $@

# Очистка
clean:
	rm -rf $(OBJ_DIR)/*
	rm -f $(EXE) $(LIB) $(TEST_EXE)
