C = gcc
INCLUDES = -I src \
	-I src/parser \
	-I src/scanner \
	-I src/std
CFLAGS = $(INCLUDES) --std=c11
DEBUG_FLAGS = -g -O0 -Wall -Wextra -save-temps -Wshadow -Wfloat-equal -Wundef -fsanitize=address
RELEASE_FLAGS = -O3

OUT_OBJ = app

BUILD = build
SOURCES = $(shell find . -name "*.c")

$(OUT_OBJ):
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) $(SOURCES) -o $(BUILD)/$@
debug:
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(SOURCES) -o $(BUILD)/$@
