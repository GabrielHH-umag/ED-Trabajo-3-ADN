CC=gcc
GRUPO=G3
NTAR=3

SRC_DIR=src
INC_DIR=include
OBJ_DIR=build/obj
BIN_DIR = build/bin
DOCS_DIR=docs
TEST_DIR=test

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))
INCLUDE=-I./$(INC_DIR)/
LIBS=
#LIBS= -lm

CFLAGS=-Wall -Wextra -Wpedantic -O3
LDFLAGS=-Wall -lm
# Extra, detectar entorno MSYS2
IS_MSYS2 := $(findstring MSYS,$(MSYSTEM))$(findstring MINGW,$(MSYSTEM))
# Detectar sistema operativo

ifeq ($(OS),Windows_NT)
    ifeq ($(IS_MSYS2),)
        # Windows CMD
        EXEC=adn.exe
        TEST_EXEC=test.exe
        SHELL := cmd.exe
        RM = del /Q
        RMDIR = rmdir /S /Q
        MKDIR = if not exist
        SEP = \\
        RUN = .\\
    
    else
        # MSYS2 o MinGW (terminal tipo Unix sobre Windows)
        EXEC=adn.exe
        TEST_EXEC=test.exe
        SHELL := /usr/bin/bash
        RM = rm -f
        RMDIR = rm -rf
        MKDIR = mkdir -p
        SEP = /
        RUN = ./
    
    endif
else
    # Linux o Mac
    EXEC=adn
    TEST_EXEC=test
    SHELL := /bin/sh
    RM = rm -f
    RMDIR = rm -rf
    MKDIR = mkdir -p
    SEP = /
    RUN = ./
endif

all: folders $(BIN_DIR)/$(EXEC)
	@echo "Proyecto compilado correctamente."

$(BIN_DIR)/$(EXEC): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) -o $@  $(INCLUDE) $(LIBS) $(LDFLAGS)
	@echo "Ejecutable generado: $@"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | folders
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE)
	@echo "Compilado: $< -> $@"

.PHONY: folders
folders:
ifeq ($(OS),Windows_NT)
	$(MKDIR) $(SRC_DIR) mkdir $(SRC_DIR)
	$(MKDIR) build\\obj mkdir build\\obj
	$(MKDIR) build\\bin mkdir build\\bin
	$(MKDIR) $(INC_DIR) mkdir $(INC_DIR)
	$(MKDIR) $(DOCS_DIR) mkdir $(DOCS_DIR)
	$(MKDIR) $(TEST_DIR) mkdir $(TEST_DIR)
	@echo "Directorios creados."
else
	$(MKDIR) $(SRC_DIR) $(OBJ_DIR) $(BIN_DIR) $(INC_DIR) $(DOCS_DIR) $(TEST_DIR)
	@echo "Directorios creados."
endif

.PHONY: clean
clean:
ifeq ($(OS),Windows_NT)
ifeq ($(IS_MSYS2),)
	# --- Windows CMD ---
	-$(RM) $(OBJ_DIR)$(SEP)*.o >nul 2>&1
	-$(RM) $(BIN_DIR)$(SEP)$(EXEC) >nul 2>&1
	-$(RM) $(BIN_DIR)$(SEP)$(TEST_EXEC) >nul 2>&1
else
	# --- MSYS2 / MinGW ---
	-$(RM) $(OBJ_FILES)
	-$(RM) $(BIN_DIR)/$(EXEC)
	-$(RM) $(BIN_DIR)/$(TEST_EXEC)
endif
else
	# --- Linux / Mac ---
	-$(RM) $(OBJ_FILES)
	-$(RM) $(BIN_DIR)/$(EXEC)
	-$(RM) $(BIN_DIR)/$(TEST_EXEC)
endif
	@echo "Archivos compilados eliminados."

.PHONY: send
send:
ifeq ($(OS),Windows_NT)
	powershell -Command "Compress-Archive -Path @( 'Makefile', '$(SRC_DIR)', '$(INC_DIR)', '$(DOCS_DIR)' ) -DestinationPath '$(GRUPO)-$(NTAR).zip' -Force"
	@echo "Archivo comprimido generado: $(GRUPO)-$(NTAR).zip"
else
	tar czf $(GRUPO)-$(NTAR).tgz --transform 's,^,$(GRUPO)-$(NTAR)/,' Makefile $(SRC_DIR) $(INC_DIR) $(DOCS_DIR)
	@echo "Archivo comprimido generado: $(GRUPO)-$(NTAR).tgz"
endif


.PHONY: docs
docs:
	doxygen Doxyfile
	@echo "Documentación generada en docs/html/index.html"

.PHONY: rebuild
rebuild: clean all
	@echo "Proyecto recompilado completamente."

.PHONY: run
run:

	@$(RUN)$(BIN_DIR)$(SEP)$(EXEC)