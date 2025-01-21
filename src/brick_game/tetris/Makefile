CC=gcc
CFLAGS=-Wall -Werror -Wextra -std=c11
GCOV_FLAGS=-fprofile-arcs -ftest-coverage -fPIC
LIB=tetris.a
CEXE=tetris_test
BACKEND_LIB=backend.a
BACKEND_EXE=backend_test

#============= FLAGS FOR OS ========================================================
UNAME:=$(shell uname -s)
ifeq ($(UNAME),Linux)
	LDFLAGS=-lcheck -lsubunit -lm -lrt -lpthread -lncurses
	OPEN_CM=xdg-open
endif
ifeq ($(UNAME),Darwin)
	LDFLAGS=-lcheck
	OPEN_CM=open
endif

#============= ALL ==================================================================
all: clean build

#============= BUILD ================================================================
build: clean ${LIB}
	${CC} ${CFLAGS}  ${LIB} -o ${CEXE} ${LDFLAGS}

#============= LIBRARY ============================================================
tetris.a: 
	${CC} ${CFLAGS} -c tetris.c ../../gui/cli/frontend.c backend.c fsm.c
	ar rc ${LIB} *.o
	ranlib ${LIB}
	rm -f *.o

backend.a: 
	${CC} ${CFLAGS} -c backend.c fsm.c
	ar rc ${BACKEND_LIB} *.o
	ranlib ${BACKEND_LIB}
	rm -f *.o

#============= TEST =============================================================
test: clean tetris.a
	$(CC) ${CFLAGS} ../../test_Tetris_C/test_Tetris_C.c ${LIB} -o ${CEXE} ${LDFLAGS}
	./${CEXE}

# Тестирование только бэкенда
test_backend: clean backend.a
	$(CC) $(CFLAGS) ../../test_Tetris_C/test_Tetris_C.c backend.a fsm.c tetris.c -o $(CEXE) $(LDFLAGS)
	./$(CEXE)


# Создание отчёта покрытия для бэкенда
gcov_backend_report: clean
	${CC} ${CFLAGS} -c backend.c fsm.c ${GCOV_FLAGS}
	ar rc ${BACKEND_LIB} *.o
	ranlib ${BACKEND_LIB}
	rm -f *.o
	$(CC) ${CFLAGS} ../../test_Tetris_C/test_Tetris_C.c ${BACKEND_LIB} -o ${BACKEND_EXE} ${LDFLAGS} ${GCOV_FLAGS}
	mkdir report
	./${BACKEND_EXE} >> report/backend_tests.txt
	lcov -t "gcov_backend" -o gcov_backend.info --no-external -c -d .
	genhtml -o report/ gcov_backend.info
	$(OPEN_CM) report/index.html
	rm -rf *.gcda *.gcno *.info

#============= GCOV =============================================================
gcov_tetris.a: clean
	${CC} ${CFLAGS} -c tetris.c frontend.c backend.c fsm.c ${GCOV_FLAGS}
	ar rc ${LIB} *.o
	ranlib ${LIB}
	rm -f *.o

gcov_report: clean gcov_tetris.a
	$(CC) ${CFLAGS} ../../test_Tetris_C/test_Tetris_C.c ${LIB} -o ${CEXE} ${LDFLAGS} ${GCOV_FLAGS}
	mkdir report
	./${CEXE} >> report/tetris_tests.txt
	lcov -t "gcov_test" -o gcov_test.info --no-external -c -d .
	genhtml -o report/ gcov_test.info
	$(OPEN_CM) report/index.html
	rm -rf *.gcda *.gcno *.info

#============= STYLE =============================================================
cppcheck:
	cppcheck --enable=all --std=c11 --language=c *.c

fsanitize: clean tetris.a
	$(CC) ${CFLAGS} -fsanitize=address -fsanitize=leak -fsanitize=undefined ../test_Tetris_C/test_Tetris_C.c ${LIB} -o ${CEXE}_fsanitize ${LDFLAGS}
	./${CEXE}_fsanitize

valgrind: clean tetris.a
	$(CC) ${CFLAGS} ../test_Tetris_C/test_Tetris_C.c ${LIB} -o ${CEXE} ${LDFLAGS}
	valgrind -s --leak-check=full --track-origins=yes --show-reachable=yes ./$(CEXE)

#=========== STYLE =================================================================
format_check:
	cp ../../materials/linters/.clang-format ./.clang-format
	clang-format -n ../test_Tetris_C/test_Tetris_C.c *.c *.h
	rm -f .clang-format

format:
	cp ../../materials/linters/.clang-format ./.clang-format
	clang-format -i ../test_Tetris_C/test_Tetris_C.c *.c *.h
	rm -f .clang-format

#========== CLEAN ==================================================================
clean:
	rm -rf *.o
	rm -rf *.out
	rm -rf *.a
	rm -rf tetris
	rm -rf tetris_test
	rm -rf tetris_test_fsanitize
	rm -rf backend_test
	rm -rf *.gcno
	rm -rf *.gcda
	rm -rf *.gcov
	rm -rf *.log
	rm -rf *.html
	rm -rf *.css
	rm -rf report
	rm -rf *.dSYM
	rm -rf *.info

