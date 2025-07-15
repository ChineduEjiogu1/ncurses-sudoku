# Compiler & Flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Iinclude

# Linker flags: use wide-character ncurses on Windows/MSYS2
LDFLAGS = -lncursesw

# Directories
SRCDIR = src
OBJDIR = obj
INCDIR = include

# Source and Object Files
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Output binary
TARGET = sudoku

# Build Rules
.PHONY: all clean install

all: $(TARGET)

# Link final binary
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# Compile each .c file to .o in obj/
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Ensure obj/ exists
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Clean build artifacts
clean:
	rm -rf $(OBJDIR) $(TARGET)

# Optional install rule
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/