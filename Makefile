CC = gcc
CFLAGS = -Wall -Wextra -I./include -I./src/engine -I./src/engine/input -I./src/engine/rendering -I./src/engine/window
LDFLAGS = -lgdi32 -lopengl32

SRC_DIR = src
ENGINE_DIR = $(SRC_DIR)/engine
BEHAVIOR_DIR = $(ENGINE_DIR)/behavior
INPUT_DIR = $(ENGINE_DIR)/input
RENDERING_DIR = $(ENGINE_DIR)/rendering
WINDOW_DIR = $(ENGINE_DIR)/window
BUILD_DIR = build

SRC = src/main.c \
      $(ENGINE_DIR)/engine.c \
      $(INPUT_DIR)/keyboard.c \
      $(INPUT_DIR)/mouse.c \
      $(RENDERING_DIR)/renderer.c \
	  $(RENDERING_DIR)/sprite.c \
	  $(BEHAVIOR_DIR)/behavior.c \
      $(WINDOW_DIR)/window.c

OBJ = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC))

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
