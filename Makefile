CC = gcc
INCLUDES = -Iinclude \
	-Isrc/parser \
	-Isrc/scanner \
	-Isrc/std
CFLAGS = $(INCLUDES) --std=c11 -lm
DEBUG_FLAGS = -g -O0 -Wall -Wextra -save-temps -Wshadow -Wfloat-equal -Wundef -fsanitize=address
RELEASE_FLAGS = -O3

OUT_OBJ = app

BUILD = build
SOURCES = $(shell find . -name "*.c")

$(OUT_OBJ):
	mkdir -p $(BUILD)
	$(CC) $(RELEASE_FLAGS) $(SOURCES) -o $(BUILD)/$@ $(CFLAGS)
debug:
	mkdir -p $(BUILD)
	$(CC) $(DEBUG_FLAGS) $(SOURCES) -o $(BUILD)/$@ $(CFLAGS)
