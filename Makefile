# Detectar si estamos en Windows (MSYS, MINGW, etc.)
ifeq ($(OS),Windows_NT)
    EXE = bio.exe
    RM = del /Q
    SHELL := cmd.exe
    FIXPATH = $(subst /,\,$1)
else
    EXE = bio
    RM = rm -f
    FIXPATH = $1
endif

CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -g -Iinclude

SRCDIR  = src
BUILDDIR = build

SRC     = $(SRCDIR)/main.c \
          $(SRCDIR)/bio_commands.c \
          $(SRCDIR)/bio_func.c

OBJ     = $(SRC:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)

.PHONY: all clean run windows linux

all: $(EXE)

# Crear carpeta build si no existe
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Compilar objetos
$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -c $(call FIXPATH,$<) -o $(call FIXPATH,$@)

# Enlazar
$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $(call FIXPATH,$@) $^

# Ejecutar (automatico según SO)
run: $(EXE)
	./$(EXE)

# Limpiar objetos y binarios
clean:
	$(RM) $(call FIXPATH,$(BUILDDIR)\*.o) 2>nul || true
	$(RM) $(call FIXPATH,$(EXE)) 2>nul || true
