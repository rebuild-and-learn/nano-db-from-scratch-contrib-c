# Makefile to compile nano db

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -pedantic -Wno-strict-prototypes -std=c99 -v

# Source file
SRC = main.c pageblock.c

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

