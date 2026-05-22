CC      = gcc
CFLAGS  = -Wall -Wextra -pedantic -std=c11 -Iinclude
TARGET  = build/hashvault
SRCS    = src/main.c src/hash.c src/auth.c

all: build $(TARGET)

build:
	mkdir -p build

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)
	@echo "Build successful → $(TARGET)"

clean:
	rm -rf build/

run: all
	./$(TARGET)

.PHONY: all clean run
