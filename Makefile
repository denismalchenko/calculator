PROJECT_NAME = SmartCalc_v2.0
# commands
CC = g++
RM := rm -rf
MKDIR := mkdir -p
# flag and parameters
CFLAGS = -Wall -Werror -Wextra
CPPFLAGS= -std=c++17 -O2 -lstdc++
LINUX_FLAGS =-lrt -lpthread -lm
GCOV_FLAGS =--coverage
TEST_FLAGS = -lgtest
# paths and filenames
TEST_DIR = test
MODEL_DIR = model
CONTROL_DIR = controller
VIEW_DIR = view
BUILD_DIR = build
DOCS_DIR = docs
DOCS_HTML = $(DOCS_DIR)/html
TEST_FILES = $(TEST_DIR)/test_*.cc
C_FILES = $(MODEL_DIR)/*.cc
H_FILES = $(MODEL_DIR)/*.h
TEST_TARGET = testing
REPORTS = reports

all: install

OS := $(shell uname -s)
USERNAME=$(shell whoami)

ifeq ($(OS),Linux)
  OPEN_CMD = xdg-open
  APP = $(BUILD_DIR)/SmartCalcV2
endif
ifeq ($(OS),Darwin)
	OPEN_CMD = open
	APP = build/SmartCalcV2.app/Contents/MacOS/SmartCalcV2
endif

install: 
	$(MKDIR) $(BUILD_DIR) && cd $(BUILD_DIR) && qmake .. 
	cd $(BUILD_DIR) && make
	@$(MKDIR) $(HOME)/Desktop/SmartCalc_v2.0/
	@cp -rf $(APP) $(HOME)/Desktop/SmartCalc_v2.0/
	make clean

uninstall: clean
	@$(RM) $(HOME)/Desktop/SmartCalc_v2.0

dist: clean
	cd ..; $(MKDIR) $(PROJECT_NAME)
	cp -r ./* ../$(PROJECT_NAME)
	cd ..; tar -cvf $(PROJECT_NAME).tar.gz $(PROJECT_NAME)
	@$(RM) ../$(PROJECT_NAME)
	@echo "-- Distribution: $(PROJECT_NAME).tar.gz"

test: $(TEST_NAME)
	 $(CC) $(CFLAGS) $(CPPFLAGS) $(C_FILES) $(TEST_FILES) $(TEST_FLAGS) -o $(TEST_TARGET)
	./$(TEST_TARGET)

gcov_report: 
	$(CC) $(GCOV_FLAGS) $(CPPFLAGS) $(C_FILES) $(TEST_FILES) -o $(TEST_TARGET) $(TEST_FLAGS)
	./$(TEST_TARGET)
	@$(MKDIR) $(REPORTS)
	lcov -t "test" -o test.info -c -d ./ --no-external
	genhtml -o $(REPORTS) test.info
	$(OPEN_CMD) ./report/index.html

dvi:
	cd $(DOCS_DIR); doxygen Doxyfile
	$(OPEN_CMD) $(DOCS_HTML)/index.html

clean:
	@rm -f $(TEST_TARGET)
	@$(RM) ../$(PROJECT_NAME).tar.gz
	@$(RM) $(BUILD_DIR) $(REPORTS)
	@$(RM) $(DOCS_HTML)
	@$(RM) *.o *.out *.info *.gcda *.gcno
	@echo "-- All clean"

rebuild: uninstall all

style:
	cd ./$(MODEL_DIR)/; clang-format -n -style=google *.cc *.h
	cd ./$(CONTROL_DIR)/; clang-format -n -style=google *.h *.cc
	cd ./$(VIEW_DIR)/; clang-format -n -style=google *.h *.cc
	cd ./$(TEST_DIR)/; clang-format -n -style=google *.cc

format:
	cd ./$(MODEL_DIR)/; clang-format -i -style=google *.cc *.h
	cd ./$(CONTROL_DIR)/; clang-format -i -style=google *.h *.cc
	cd ./$(VIEW_DIR)/; clang-format -i -style=google *.h *.cc
	cd ./$(TEST_DIR)/; clang-format -i -style=google *.cc

.PHONY: all clean test gcov_report install uninstall dist rebuild style format
