CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
TARGET = Library.exe
TESTFILE = test3

all: $(TARGET)

$(TARGET): Library.c Library.h
	$(CC) $(CFLAGS) -o $(TARGET) Library.c

run: $(TARGET)
	./$(TARGET) $(TESTFILE)

clean:
	rm -f $(TARGET)

.PHONY: all run clean