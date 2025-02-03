# Compiler and flags
CC=clang
DEBUG_FLAGS=-std=c99 -O0 -Wall -g -Ilib
RELEASE_FLAGS=-std=c99 -O2 -Wall -march=native -Ilib
LDFLAGS=-lm
PRJ=formula
SRC_DIR=src
BUILD_DIR=build
LIB_DIR=lib

# Source files
FLEX_SRC=$(SRC_DIR)/formula.l
BISON_SRC=$(SRC_DIR)/formula.y

# Generated files from Bison and Flex
TAB_C=$(BUILD_DIR)/formula.tab.c
TAB_H=$(BUILD_DIR)/formula.tab.h
LEX_C=$(BUILD_DIR)/formula.lex.c

# Library source files
LIB_SRC=$(wildcard $(LIB_DIR)/*.c)
LIB_OBJ=$(patsubst $(LIB_DIR)/%.c,$(BUILD_DIR)/%.o,$(LIB_SRC))

# Linux build
release: CFLAGS=$(RELEASE_FLAGS) -D_GNU_SOURCE --target=x86_64-linux-gnu
release: clean generate_sources $(LIB_OBJ)
	$(CC) $(CFLAGS) -o $(PRJ) $(TAB_C) $(LEX_C) $(LIB_OBJ) $(LDFLAGS)
	
# Debug build
debug: CFLAGS=$(DEBUG_FLAGS) -D_GNU_SOURCE
debug: clean generate_sources $(LIB_OBJ)
	$(CC) $(CFLAGS) -o $(PRJ)_debug $(TAB_C) $(LEX_C) $(LIB_OBJ) $(LDFLAGS)

# Rule to generate files from Bison and Flex BEFORE compilation
generate_sources: $(TAB_C) $(LEX_C)

$(TAB_C) $(TAB_H): $(BISON_SRC)
	mkdir -p $(BUILD_DIR)
	bison -v -t --defines --report=all -Wcounterexamples -o $(TAB_C) $(BISON_SRC)

$(LEX_C): $(FLEX_SRC) $(TAB_H)
	flex -o $(LEX_C) $(FLEX_SRC)

# Rule to compile files in the lib directory
$(BUILD_DIR)/%.o: $(LIB_DIR)/%.c generate_sources
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Target to clean generated files
clean:
	rm -rf $(BUILD_DIR) $(PRJ) $(PRJ)_debug $(PRJ)_linux $(PRJ)_macos $(PRJ).exe
