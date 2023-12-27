CC=gcc
CFLAGS=-Wall -Werror -Wextra -c

TESTFLAGS := -lcheck -lpthread -pthread

UNAME=$(shell uname -s)

ifeq ($(UNAME),Linux)
    TESTFLAGS += -lsubunit -lm -lrt
    OPEN_COMMAND=firefox
endif

ifeq ($(UNAME),Darwin)
    OPEN_COMMAND=open
endif

GCOVFLAGS = -fprofile-arcs -ftest-coverage

SOURCES=object.c object_builder.c object_parser.c
OBJECTS=$(SOURCES:.c=.o)

TEST_SOURCES=$(shell find ./tests -name '*.c')

GCDA_FILES=*.gcda
GCNO_FILES=*.gcno
GCOV_FILES=*.gcov

FILES_TO_CHECK=$(shell find . -name '*.c' && find . -name '*.h')

.PHONY: all 

all: s21_obj.a

%.o: %.c
	$(CC) $(CFLAGS) $^ -o $@
	
s21_obj.a: $(OBJECTS)
	ar rcs s21_obj.a $(OBJECTS)
	ranlib s21_obj.a
	
test: test_s21_obj.a
	$(CC) $(TEST_SOURCES) -L. s21_obj.a $(TESTFLAGS) $(GCOVFLAGS) -o test.out
	./test.out
	
gcov_report: test
	lcov -t test.out -o test.info -c -d .
	genhtml -o report test.info
	$(OPEN_COMMAND) report/index.html
	

test_s21_obj.a: CFLAGS=-Wall -Werror -Wextra -c -fprofile-arcs -ftest-coverage -g
test_s21_obj.a: s21_obj.a

rebuild: clean all

clean:
	@rm -rf $(GCDA_FILES) $(GCNO_FILES) $(GCOV_FILES)
	@rm -rf $(OBJECTS)
	@rm -rf report/ *.a *.out test.info
	@rm -rf archive_3DViewer_v1
	@rm -rf archive_3DViewer_v1.tgz
	@rm -rf build

.clang-format:
	@cp ../materials/linters/.clang-format .
	
stylecheck: .clang-format
	@clang-format -n $(FILES_TO_CHECK)
	
fmt: .clang-format
	@clang-format -i $(FILES_TO_CHECK)

uninstall: clean
	rm -rf build

install: uninstall
	mkdir build
	cmake -S ./3DViewer_v1 -B ./build
	cd build; make

dist: clean
	mkdir archive_3DViewer_v1
	cp Makefile *.c *.h archive_3DViewer_v1/
	cp -R tests archive_3DViewer_v1/
	cp -R 3DViewer_v1 archive_3DViewer_v1/
	tar cvzf archive_3DViewer_v1.tgz archive_3DViewer_v1/

dvi:
	open doc.html
