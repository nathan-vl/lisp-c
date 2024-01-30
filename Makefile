C = gcc
CFLAGS = -I include --std=c11
DEBUG_FLAGS = -g -O0 -Wall -Wextra -fsanitize=address
RELEASE_FLAGS = -O3

OUT_OBJ = app

BUILD = build
SOURCES = $(wildcard src/*.c)

$(OUT_OBJ):
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) $(SOURCES) -o $(BUILD)/$@
debug:
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(SOURCES) -o $(BUILD)/$@
