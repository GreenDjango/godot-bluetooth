#!/usr/bin/make -f

# ---
#	Makefile
#
#	This file is part of the Godot Bluetooth Module project, and is made
#	available under the terms of the MIT License version.
#
# 	Copyright (C) 2022 - Godot Bluetooth Module contributors
# ---

# ---
#	set compilation tools
# ---
CC				:= g++
HEADER_DIR		:= -Iinclude/ -Imodule/
HEADER_LIBS_DIR	:= -isystem /usr/include/qt6 -isystem /usr/include/qt6/QtBluetooth -isystem /usr/include/qt6/QtCore -isystem /usr/include/qt6/QtNetwork -isystem /usr/include/qt6/QtDBus
DEFINES			:= -DGODOT_BLUETOOTH_P_X11 -DQT_BLUETOOTH_LIB -DQT_CORE_LIB -DQT_DBUS_LIB -DQT_NETWORK_LIB -DQT_NO_DEBUG
CFLAGS			:= -Wall -Wextra -W -std=c++17 -std=gnu++17 -g3 $(DEFINES) $(HEADER_DIR) $(HEADER_LIBS_DIR)
LIBS			:= -lQt6Bluetooth -lQt6Core -lQt6Network -lQt6DBus
BUILD_DIR		:= build
SRC_DIR			:= src
VERSION			:= 1.0 # $(shell git show --format="%h" -q)

# ---
#	set all binary name
# ---
NAME_BINARY		:= test.out


# ---
#	set all compil file
# ---
SRC_FILES		:= $(wildcard $(SRC_DIR)/*.cpp) \
					$(SRC_DIR)/module/bluetooth_bluez.cpp \
					$(SRC_DIR)/module/bluetooth_dummy.cpp


# ---
#	set all binary file name
# ---
DEST_FILES		:= $(basename $(SRC_FILES:$(SRC_DIR)/%=$(BUILD_DIR)/%))
DEST_OBJ		:= $(DEST_FILES:%=%.o)


# ---
#	build rule
# ---
all: $(NAME_BINARY)


# ---
#	main rule
# ---
$(NAME_BINARY): $(DEST_OBJ)
	$(CC) -o $@ $^ $(LIBS)
	@ printf "\33[00;32m%s\33[0m\n" "COMPILING STATUS -> COMPLETE"
	@ printf "\33[0mCurrent build version: \33[32m[ %s ]\33[0m\n" $(VERSION)


# ---
#	obj build
# ---

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@ mkdir -p $(dir $@)
	$(CC) -c -o $@ $< $(CFLAGS)


# ---
# 	clean rule
# ---
clean:
	rm -rf $(BUILD_DIR)
	printf "\33[00;33mREPOSITORY CLEAN ... \33[0m\n"

fclean:
	rm -rf $(BUILD_DIR) $(NAME_BINARY)
	printf "\33[01;35mREPOSITORY FCLEAN ... \33[0m\n"

re: fclean all


# ---
# 	function
# ---
.PHONY: all clean re fclean

.SILENT: clean fclean re