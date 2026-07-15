CC = gcc
CFLAGS = -Wall -Wextra -Werror -O2
TARGET = day_calculator
SRC = src/main.c

ifeq ($(OS),Windows_NT)
    RM = del /Q /F
    TARGET_BIN = $(TARGET).exe
    FIX_PATH = $(subst /,\,$1)
else
    RM = rm -f
    TARGET_BIN = $(TARGET)
    FIX_PATH = $1
endif

all: $(TARGET_BIN)

$(TARGET_BIN): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET_BIN)

clean:
	$(RM) $(call FIX_PATH,$(TARGET_BIN))

.PHONY: all clean
