###################### ALL FLAGS ####################

CC = g++ #-fsanitize=address
CFLAGS = -Wall -Werror -Wextra -std=c++20
GCOV_FLAGS = -fprofile-arcs -ftest-coverage

OS := $(shell uname)

ifeq ($(OS),Linux)
    FLAGS_TEST = -fPIC -I/opt/Qt/6.9.0/gcc_64/include \
	-I/opt/Qt/6.9.0/gcc_64/include/QtCore \
	-I/opt/Qt/6.9.0/gcc_64/include/QtGui \
	-I/opt/Qt/6.9.0/gcc_64/include/QtWidgets \
	-I/opt/Qt/6.9.0/gcc_64/include/QtOpenGL \
	-I/opt/Qt/6.9.0/gcc_64/include/QtOpenGLWidgets

    LDFLAGS = -L/opt/Qt/6.9.0/gcc_64/lib -lQt6Core -lQt6Gui -lQt6Widgets -lQt6OpenGL -lQt6OpenGLWidgets

	LIBS = -lgtest -lm -lsubunit -pthread $(LDFLAGS)
endif

# если не запускаются тесты -> прописать в терминале
# export LD_LIBRARY_PATH=/opt/Qt/6.9.0/gcc_64/lib:$LD_LIBRARY_PATH 

###################### TARGETS ######################

TARGET = 3d_viewer
TARGET_TEST = viewer_test

###################### PATHS / SOURCES ###############

BUILD_DIR = build
INSTALL_DIR = install
SAVE_IMAGE_DIR = saved_images
SAVE_GIF_DIR = saved_screencasts
GCOV_DIR = gcov_report
DOCS_DIR = documentation

ALL_SRC = $(wildcard *.cpp *.h  */*.cpp */*.h */*/*.cpp */*/*.h)
TEST_SRC = tests/main_test.cpp

SRC_TEST = $(wildcard model/*.cpp controller/*.cpp model/matrix/*.cpp)
OBJS_TEST = $(SRC_TEST:.cpp=.o)
CPPFLAGS_TEST = -DFOR_TESTS $(FLAGS_TEST)

######################################################

VERSION = 2.0

RM:= rm -rf
MK:= mkdir -p

CPP = cppcheck --enable=all --suppress=missingIncludeSystem --std=c++20 --language=c++

######################################################

all: install

install: build
	@$(MK) $(INSTALL_DIR)
	@cp -p $(BUILD_DIR)/$(TARGET) $(INSTALL_DIR)
	@echo "-- \e[33mInstall complete.\e[0m"
	@echo "-- Use \e[32mmake run\e[0m to start the program."

uninstall:
	@$(RM) $(INSTALL_DIR)
	@clear
	@echo "-- \e[31mUninstall complete.\e[0m"

clean: uninstall delete
	@$(RM) $(BUILD_DIR)
	@$(RM) *.tar.gz
	@$(RM) $(DOCS_DIR)
	@$(RM) $(TARGET_TEST)
	@$(RM) $(GCOV_DIR)
	@$(RM) coverage.info
	@find model controller -type f -name "*.o" -exec rm -f {} \;
	@find . -name "*.gcda" -exec rm -f {} \;
	@find . -name "*.gcno" -exec rm -f {} \;
	@find . -name "*.gcov" -exec rm -f {} \;
	@echo "-- \e[31mClean complete.\e[0m"

dvi:
	@doxygen
	@clear
	@open $(DOCS_DIR)/html/index.html || echo "Please open $(DOCS_DIR)/html/index.html manually."
	@echo "-- \e[32mDvi complete.\e[0m"

dist:
	@if [ ! -d "$(TARGET)-$(VERSION)" ]; then \
		$(MK) $(TARGET)-$(VERSION); \
	fi
	@cp -r controller model view Makefile 3d_viewer.pro $(TARGET)-$(VERSION)
	@tar -czf $(TARGET)-$(VERSION).tar.gz $(TARGET)-$(VERSION)
	@$(RM) $(TARGET)-$(VERSION)
	@echo "-- \e[33mDist complete. Archive name '"$(TARGET)-$(VERSION).tar.gz"'\e[0m"

tests: $(OBJS_TEST)
	@$(CC) $(CFLAGS) $(CPPFLAGS_TEST) $(TEST_SRC) $(OBJS_TEST) -o $(TARGET_TEST) $(LIBS)
	LD_LIBRARY_PATH=/opt/Qt/6.9.0/gcc_64/lib:$$LD_LIBRARY_PATH ./$(TARGET_TEST)

%.o: %.cpp
	@$(CC) $(CFLAGS) $(CPPFLAGS_TEST) -c $< -o $@

build:
	@if [ ! -d "$(BUILD_DIR)" ]; then \
		$(MK) $(BUILD_DIR); \
	fi
	@cd $(BUILD_DIR) && /opt/Qt/6.9.0/gcc_64/bin/qmake ../.
	@cd $(BUILD_DIR) && make
	@clear
	@echo "-- \e[33mBuild complete.\e[0m"

run:
	@./$(INSTALL_DIR)/$(TARGET)

gcov_report: CPPFLAGS_TEST += $(GCOV_FLAGS)
gcov_report: tests
	@$(MK) $(GCOV_DIR)
	@geninfo . -o coverage.info
	@lcov --remove coverage.info '/usr/*' '/opt/Qt/6.9.0/gcc_64/include/QtCore/*' '/opt/Qt/6.9.0/gcc_64/include/QtGui/*' -o coverage.info
	@genhtml coverage.info --output-directory gcov_report
	@echo "-- \e[33mGcov HTML report generated in $(GCOV_DIR)/index.html.\e[0m"
	@xdg-open $(GCOV_DIR)/index.html || open $(GCOV_DIR)/index.html || echo "Please open $(GCOV_DIR)/index.html manually."

delete:
	@$(RM) settings.txt
	@if [ -d "$(SAVE_IMAGE_DIR)" ]; then \
		$(RM) $(SAVE_IMAGE_DIR); \
	fi
	@if [ -d "$(SAVE_GIF_DIR)" ]; then \
		$(RM) $(SAVE_GIF_DIR); \
	fi
	@echo "-- \e[31mDelete complete. The settings file and saved images have been deleted.\e[0m"

###################### CLANG-FORMAT ######################

clang:
	@clang-format --style=Google -i $(ALL_SRC)
	@clang-format --style=Google -n $(ALL_SRC)
	@echo ""
	@echo "-- \e[33mClang-format complete.\e[0m"
	@echo ""

install_gtest:
	git clone https://github.com/google/googletest
	cd googletest && mkdir -p build && cd build
	sudo apt install cmake g++
	sudo apt install googletest
	cd ./googletest/googletest && cmake .. && make && sudo make install

.PHONY: all install uninstall clean dvi dist tests build run delete clang gcov_report
