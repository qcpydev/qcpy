CC = gcc
CFLAGS = -Wall -Wextra -fPIC -Iinclude
LDFLAGS = -shared

# Binary output
TARGET = core.so

# Find all .c files and their corresponding .o files
SRC := $(shell find . -type f -name '*.c')
OBJ := $(SRC:.c=.o)

# Default rule
all: $(TARGET)

# Link the binary
$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

# Compile .c files into .o files in the same directories
%.o: %.c
	$(CC) $(CFLAGS) -g -c -o $@ $<

# Clean rule
clean:
	find . -type f -name '*.o' -delete
	rm -f $(TARGET)

.PHONY: all clean
