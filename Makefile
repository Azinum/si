# Makefile - simple interpreter

include config.mk

all: prepare local run

clean:
	rm build/*

prepare:
	@if [ ! -d $(BUILD_DIR) ]; then \
		mkdir $(BUILD_DIR); \
	fi \

local:
	$(CC) $(FLAGS) $(FLAGS_LOCAL)

install:
	$(CC) $(FLAGS) $(FLAGS_RELEASE)

build_debug:
	$(CC) $(FLAGS) $(FLAGS_DEBUG)

debug: build_debug run_debug

run:
	./$(BUILD_DIR)/$(PROGRAM_NAME)

run_debug:
	gdb ./$(BUILD_DIR)/$(PROGRAM_NAME)
