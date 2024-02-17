# Makefile to compile nano db

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -pedantic -std=c99

# Source file
SRC = pageblock.c

OBJ = $(SRC:.c=.o)

# Executable name
EXEC = pageblock

# Default target
all: $(EXEC)

# Compile
$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $<

%.o: %.c pageblock.h
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean the generated files
clean:
	rm -rf $(EXEC) $(OBJ)

