

BUILD_DIR ?= build
.PHONY: build clean
default: run


build: $(BUILD_DIR)/Makefile
	cd $(BUILD_DIR) && make --trace

$(BUILD_DIR)/Makefile: Makefile
	mkdir -p $(BUILD_DIR) && cmake -S . -B $(BUILD_DIR) -DCMAKE_TOOLCHAIN_FILE=$(VCPKG_HOME)/scripts/buildsystems/vcpkg.cmake

clean:	
	rm -rf $(BUILD_DIR)


run: build
	$(BUILD_DIR)/main hello world

run_gdb: build
	gdb --args  env HELLO=world $(BUILD_DIR)/main  hello world

run_strace: build
	strace $(BUILD_DIR)/main
