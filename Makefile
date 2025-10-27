CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
SRCS = Library.c
TARGET = Library.exe

ifeq ($(OS),Windows_NT)
RUN = .\$(TARGET)
RM  = del /Q
else
RUN = ./$(TARGET)
RM  = rm -f
endif

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

run: $(TARGET)
	$(RUN)

clean:
	$(RM) $(TARGET)