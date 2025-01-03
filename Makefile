# Variabili di configurazione
CC=gcc
CFLAGS=-O0 -Wall -ggdb -Ilib
LDFLAGS=-lm
PRJ=forma
SRC_DIR=src
BUILD_DIR=build
TEST_DIR=tests
LIB_DIR=lib

# File di sorgente
FLEX_SRC=$(SRC_DIR)/forma.l
BISON_SRC=$(SRC_DIR)/forma.y

# File di output generati da bison e flex
TAB_C=$(BUILD_DIR)/forma.tab.c
TAB_H=$(BUILD_DIR)/forma.tab.h
LEX_C=$(BUILD_DIR)/forma.lex.c

# File nella directory lib
LIB_SRC=$(wildcard $(LIB_DIR)/*.c)
LIB_OBJ=$(patsubst $(LIB_DIR)/%.c,$(BUILD_DIR)/%.o,$(LIB_SRC))

# Target principale
$(PRJ): $(TAB_C) $(LEX_C) $(LIB_OBJ)
	$(CC) $(CFLAGS) -o $(PRJ) $(TAB_C) $(LEX_C) $(LIB_OBJ) $(LDFLAGS)

# Regola per compilare i file nella directory lib
$(BUILD_DIR)/%.o: $(LIB_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Regola per generare i file da bison
$(TAB_C) $(TAB_H): $(BISON_SRC)
	mkdir -p $(BUILD_DIR)
	bison -v -t --defines --report=all -Wcounterexamples -o build/forma.tab.c src/forma.y

# Regola per generare il file da flex
$(LEX_C): $(FLEX_SRC) $(TAB_H)
	flex -o $(LEX_C) $(FLEX_SRC)

# Target per pulire i file generati
clean:
	rm -rf $(BUILD_DIR) $(PRJ)

# Regola per eseguire il programma con tutti i file nella cartella tests
run: $(PRJ)
	for f in $(TEST_DIR)/*; do \
		./$(PRJ) $$f; \
	done

