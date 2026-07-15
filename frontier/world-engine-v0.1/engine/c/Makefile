CC ?= cc
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -Wpedantic -Werror
CPPFLAGS ?= -Iinclude
BUILD := build

LIBOBJ := $(BUILD)/world_engine.o

.PHONY: all clean test demo

all: test demo

$(BUILD):
	mkdir -p $(BUILD)

$(LIBOBJ): src/world_engine.c include/world_engine.h | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD)/test_engine: tests/test_engine.c $(LIBOBJ)
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@

$(BUILD)/world_demo: src/demo.c $(LIBOBJ)
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@

test: $(BUILD)/test_engine
	./$(BUILD)/test_engine

demo: $(BUILD)/world_demo
	./$(BUILD)/world_demo $(BUILD)/golden_pose.jsonl

clean:
	rm -rf $(BUILD)
