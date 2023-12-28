CXX							= g++
CXXFLAGS				= -std=c++17 -Wall -Wextra -Werror -fsanitize=address,undefined -I DeadInside/
LDFLAGS					= -lgtest_main -lgtest -lpthread 
VALGRIND_FLAGS	= --log-file="valgrind.txt" --track-origins=yes --trace-children=yes --leak-check=full --leak-resolution=med
GCOVFLAGS 			= -fprofile-arcs -ftest-coverage

PROJECT_DIR			= DeadInside/
PROJECT 				= DeadInside

MODEL_DIR 			= $(PROJECT_DIR)model/
CONTROLLER_DIR	= $(PROJECT_DIR)controller/
VIEW_DIR				= $(PROJECT_DIR)view/
TEST_DIR				= $(PROJECT_DIR)model/tests/

BUILD_DIR				= build/
ARCH_NAME				= 

MODEL_SRC				= $(wildcard $(MODEL_DIR)*.cc)
TEST_SRC				= $(wildcard $(TEST_DIR)*.cc)

FILES_TO_CHECK	= $(shell find $(MODEL_DIR) $(CONTROLLER_DIR) $(VIEW_DIR) -name '*.cc' && find $(MODEL_DIR) $(CONTROLLER_DIR) $(VIEW_DIR) -name '*.h' )

.PHONY: install uninstall open_calc dist dist_remove tests rebuild clean stylecheck fmt dvi dvi_remove

all: aboba

aboba:
	$(CXX) $(CXXFLAGS) $(MODEL_SRC) main.cc -o grinch

install:
	@mkdir $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake ../$(PROJECT_DIR)
	make -C $(BUILD_DIR)

open_viewer:
	@./$(shell find $(BUILD_DIR) -name "$(PROJECT)")

uninstall:
	@rm -rf build*

dist:
	@mkdir $(ARCH_NAME)
	@cp -r $(PROJECT_DIR) Makefile $(ARCH_NAME)/
	tar cvzf $(ARCH_NAME).tgz $(ARCH_NAME)/
	@rm -rf $(ARCH_NAME)

dist_remove:
	@rm -rf $(ARCH_NAME)*

dvi:
	doxygen doxconf
	@open documentation/html/index.html

dvi_remove:
	@rm -rf documentation

tests:
	$(CXX) $(CXXFLAGS) $(TEST_SRC) $(MODEL_SRC) -o test $(LDFLAGS)
	./test

rebuild: clean all

gcovr_report: clean
	$(CXX) $(CXXFLAGS) $(TEST_SRC) $(MODEL_SRC) -o test $(LDFLAGS) $(GCOVFLAGS)
	./test
	mkdir gcovr/
	gcovr --html-details gcovr/report.details.html
	rm -rf ./*.gc*
	open gcovr/report.details.html

clean:
	@rm -rf test
	@rm -rf gcovr
	@rm -rf report
	@rm -rf *.info
	@rm -rf ./*.gc*
	@rm -rf *.txt
	@rm -rf *.dSYM

fmt:
	@clang-format -i --style=Google $(FILES_TO_CHECK)
	
stylecheck:
	@clang-format -n --style=Google $(FILES_TO_CHECK)
	@cpplint --filter=-legal/copyright,-build/header_guard,-runtime/references $(FILES_TO_CHECK)

cppcheck:
	cppcheck --std=c++17 --language=c++ --enable=all --suppress=missingIncludeSystem $(FILES_TO_CHECK)