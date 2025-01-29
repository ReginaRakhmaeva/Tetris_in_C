# Компилятор и флаги
CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -fprofile-arcs -ftest-coverage
LDFLAGS = -lncurses -lcheck -lm -lpthread -lsubunit

# Папки
OBJ_DIR = ../build
SRC_DIR = .
BRICK_GAME_DIR = $(SRC_DIR)/brick_game/tetris
GUI_CLI_DIR = $(SRC_DIR)/gui/cli
TEST_DIR = $(SRC_DIR)/tests
REPORT_DIR = $(SRC_DIR)/gcov_report

# Цели
EXE = ../tetris
LIB = ../libtetris.a
TEST_EXE = ../tetris_test

# Исходные и объектные файлы для основной программы
SRC_FILES = $(wildcard $(BRICK_GAME_DIR)/*.c) $(wildcard $(GUI_CLI_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

# Исходные файлы для тестов (только для brick_game/tetris)
TEST_SRC = $(TEST_DIR)/tests.c  $(TEST_DIR)/tests.h
TEST_OBJ = $(OBJ_DIR)/test.o
TEST_SRC_FILES = $(wildcard $(BRICK_GAME_DIR)/*.c)
TEST_OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(TEST_SRC_FILES))

# Каталог установки
PREFIX = /usr/local
BINDIR = $(PREFIX)/bin

# Основная цель
all: clean $(EXE)

# Сборка исполняемого файла
$(EXE): $(OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Правило для объектных файлов
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)/$(dir $<) 
	$(CC) $(CFLAGS) -c $< -o $@

# Создание библиотеки
lib: $(LIB)

$(LIB): $(OBJ_FILES)
	ar rc $@ $^
	ranlib $@

# Цель для тестов
test: clean $(TEST_EXE)
	./$(TEST_EXE)

# Сборка исполняемого файла для тестов (только для brick_game/tetris)
$(TEST_EXE): $(TEST_OBJ) $(TEST_OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Компиляция файла с тестами
$(TEST_OBJ): $(TEST_SRC)
	@mkdir -p $(OBJ_DIR)/$(dir $<)
	$(CC) $(CFLAGS) -c $< -o $@

# Цель для отчета покрытия кода
gcov_report: clean $(TEST_EXE)
	@mkdir -p $(REPORT_DIR)
	./$(TEST_EXE)
	gcov -o $(OBJ_DIR) $(TEST_OBJ_FILES) > $(REPORT_DIR)/coverage.txt
	lcov --capture --directory $(OBJ_DIR) --output-file $(REPORT_DIR)/coverage.info
	genhtml $(REPORT_DIR)/coverage.info --output-directory $(REPORT_DIR)
	@echo "HTML report generated in $(REPORT_DIR)"

# Цель для valgrind (проверка на утечки памяти в brick_game/tetris)
valgrind: clean $(LIB)
	$(CC) $(CFLAGS) $(TEST_DIR)/*.c $(BRICK_GAME_DIR)/*.c -o $(TEST_EXE) $(LDFLAGS)
	valgrind -s --leak-check=full --track-origins=yes --show-reachable=yes ./$(TEST_EXE)

# Цель для cppcheck (проверка исходников всего проекта)
cppcheck:
	cppcheck --enable=all --std=c11 --language=c  $(BRICK_GAME_DIR)/*.c $(GUI_CLI_DIR)/*.c 

# Цель для clang-format
format_check:
	cp ../materials/linters/.clang-format ../src/.clang-format
	clang-format -n tests/*.c tests/*.h brick_game/tetris/*.c brick_game/tetris/*.h gui/cli/*.c gui/cli/*.h
	rm -f .clang-format

# Установка
install: $(EXE)
	@mkdir -p $(BINDIR)
	cp $(EXE) $(BINDIR)
	@echo "Installed $(EXE) to $(BINDIR)"

# Удаление
uninstall:
	rm -f $(BINDIR)/$(notdir $(EXE))
	@echo "Uninstalled $(EXE) from $(BINDIR)"

# Создание документации
dvi:
	@echo "Generating DVI documentation..."
	doxygen

# Создание архива
dist:
	mkdir -p dist
	tar -czvf dist/tetris.tar.gz $(SRC_DIR) $(TEST_DIR) Makefile
	@echo "Source archive created: dist/tetris.tar.gz"


# Очистка
clean:
	rm -rf $(OBJ_DIR)
	rm -f $(EXE) $(LIB) $(TEST_EXE)
	rm -rf *.gcda *.gcno
	rm -rf $(REPORT_DIR)
	rm -rf ../*.gcda ../*.gcno
	rm -rf dist