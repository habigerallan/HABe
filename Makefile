CC = gcc
CFLAGS = -Wall -Wextra -I./include
LDFLAGS = -lgdi32 -lopengl32

SRC_DIR = src
ENGINE_DIR = $(SRC_DIR)/engine
GRAPHICS_DIR = $(SRC_DIR)/graphics
INPUT_DIR = $(SRC_DIR)/input
MATH_DIR = $(SRC_DIR)/gmath
PLATFORM_DIR = $(SRC_DIR)/platform
BUILD_DIR = build

SRC = \
    $(SRC_DIR)/game.c \
    $(ENGINE_DIR)/engine.c \
    $(GRAPHICS_DIR)/animation.c \
	$(GRAPHICS_DIR)/camera.c \
    $(GRAPHICS_DIR)/renderer.c \
    $(GRAPHICS_DIR)/sprite.c \
    $(INPUT_DIR)/input.c \
    $(MATH_DIR)/gmath.c \
    $(PLATFORM_DIR)/win32_window.c

OBJ = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC))

TARGET = main.exe

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(BUILD_DIR)
	$(CC) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(TARGET)
