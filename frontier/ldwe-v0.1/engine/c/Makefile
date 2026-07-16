CC ?= cc
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -Wpedantic -Werror
CPPFLAGS ?= -Iinclude
BUILD := build

LIBOBJ := $(BUILD)/legion_engine.o

.PHONY: all clean test demo

all: test demo

$(BUILD):
	mkdir -p $(BUILD)

$(LIBOBJ): src/legion_engine.c include/legion_engine.h | $(BUILD)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD)/test_engine: tests/test_engine.c $(LIBOBJ)
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@

$(BUILD)/legion_demo: src/demo.c $(LIBOBJ)
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@

test: $(BUILD)/test_engine
	./$(BUILD)/test_engine

demo: $(BUILD)/legion_demo
	./$(BUILD)/legion_demo $(BUILD)/golden_pose.jsonl

clean:
	rm -rf $(BUILD)
