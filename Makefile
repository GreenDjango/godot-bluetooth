#!/usr/bin/make -f
# ---
#	GODOT PROJECT, 2020
#	bluetooth module
#	Author: Theo
# ---

# ---
#	set compilation tools
# ---
CC				:= g++
HEADER_DIR		:= include/
CFLAGS			:= -Wall -Wextra -W -g3 -lbluetooth -I$(HEADER_DIR)
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
SRC_FILES		:= $(wildcard $(SRC_DIR)/*.cpp)


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
	$(CC) -o $@ $^ $(CFLAGS)
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
