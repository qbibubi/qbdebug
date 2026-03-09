BUILD_TYPE ?= Debug
BUILD_DIR := build/$(BUILD_TYPE)

TARGET := $(BUILD_DIR)/qbdbg

export CC ?= clang

.PHONY: all configure build run clean

all: build

$(BUILD_DIR)/CMakeCache.txt:
	cmake -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

configure: $(BUILD_DIR)/CMakeCache.txt

build: configure
	cmake --build $(BUILD_DIR)

run: build
	./$(TARGET)

clean:
	rm -rf build
