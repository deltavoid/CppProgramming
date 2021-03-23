

BUILD_DIR ?= build
.PHONY: build clean
#default: run_argv


build: $(BUILD_DIR)/Makefile
	cd $(BUILD_DIR) && make --trace

$(BUILD_DIR)/Makefile: Makefile
	mkdir -p $(BUILD_DIR) && cmake -S . -B $(BUILD_DIR) -DCMAKE_TOOLCHAIN_FILE=$(VCPKG_HOME)/scripts/buildsystems/vcpkg.cmake

clean:	
	rm -rf $(BUILD_DIR)


run_scoped_ptr: build
	$(BUILD_DIR)/scoped_ptr/scoped_ptr

run_unique_ptr1: build
	$(BUILD_DIR)/unique_ptr/unique_ptr1

run_unique_ptr2: build
	$(BUILD_DIR)/unique_ptr/unique_ptr2

run_unique_ptr_deleter: build
	$(BUILD_DIR)/unique_ptr/unique_ptr_deleter

run_shared_ptr1: build
	$(BUILD_DIR)/shared_ptr/shared_ptr1

run_shared_ptr2: build
	$(BUILD_DIR)/shared_ptr/shared_ptr2

run_shared_ptr_deleter: build
	$(BUILD_DIR)/shared_ptr/shared_ptr_deleter
	