CC=gcc
BUILDFLAGS=-std=c11 -Wall -Wextra -Werror
CFLAGS=$(BUILDFLAGS) -c

CMAKE_DIR=s21_3DViewer
BUILD_DIR=build
ARCH_NAME=Archive_$(CMAKE_DIR)

TESTFLAGS := -lcheck -lpthread -pthread

TESTFLAGS +=
ifeq ($(shell uname -s),Linux)
	TESTFLAGS += -lm -lrt -lsubunit
endif

GCOVFLAGS = -fprofile-arcs -ftest-coverage

TESTS_SOURCES=$(addprefix tests/, test.c)
SOURCES=$(wildcard backend/*.c)
OBJECTS=$(SOURCES:.c=.o)

FILES_TO_CHECK=$(shell find . -name '*.c' | grep -v QtGifImage && find . -name '*.h' | grep -v QtGifImage )
MAINFILE=${MAIN}

.PHONY: all tests test dvi gcov_report gcovr_report rebuild clean stylecheck fmt

all: clean install open_viewer

%.o: %.c
	$(CC) $(CFLAGS) -g $^ -o $@

install: clean
	@mkdir $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake ../$(CMAKE_DIR)
	make -C $(BUILD_DIR)

open_viewer:
	@./$(shell find build -name "$(CMAKE_DIR)")

uninstall:
	@rm -rf build

dist: clean
	cd ../ && tar czf $(ARCH_NAME).tar.gz src/

dist_remove:
	@rm -rf ../$(ARCH_NAME).tar.gz

dvi:
	doxygen doxconf
	@open documentation/html/index.html

dvi_remove:
	@rm -rf documentation

$(LIB_NAME): $(OBJECTS)
	ar rcs $(LIB_NAME) $^
	ranlib $(LIB_NAME)

test: clean 
	$(CC)  $(BUILDFLAGS) $(SOURCES) $(TESTS_SOURCES) $(GCOVFLAGS) $(TESTFLAGS) -o test.out
	rm -rf test.gc*
	./test.out

tests: test

gcov_report: test
	lcov -t test.out -o test.info -c -d . --no-external
	genhtml -o report test.info
	@open report/index.html

gcovr_report: test
	mkdir gcovr/
	gcovr --html-details gcovr/report.details.html
	@open gcovr/report.details.html

report_nohtml: test
	gcov *gcno > coverage.txt
	@open coverage.txt

rebuild: clean all

clean: 
	@rm -rf  $(OBJECTS) build* 
	@rm -rf report/ gcovr/  *.gcda *.gcno *.gcov *.out *.info *.o coverage.txt *.dSYM
	
stylecheck:
	@clang-format -n --style=Google $(FILES_TO_CHECK)
	
fmt:
	@clang-format -i --style=Google $(FILES_TO_CHECK)
