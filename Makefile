CC = gcc
CFLAGS = -municode -I$(INCLUDE)

SRC = src
INCLUDE = include
BUILD = build

TARGET = main.exe

SRCS = $(wildcard $(SRC)/*.c)
OBJS = $(patsubst $(SRC)/%.c, $(BUILD)/%.o, $(SRCS))

VPATH = $(SRC) $(INCLUDE)

.PHONY: all clean clean-all

all: $(BUILD) $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD):
	@if not exist $(BUILD) mkdir $(BUILD)

$(BUILD)/%.o: $(SRC)/%.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@if exist $(BUILD) rmdir /s /q $(BUILD)
	@if exist $(TARGET) del /q $(TARGET)

clean-all: clean all
