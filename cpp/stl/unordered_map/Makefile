

BUILD_DIR ?= build
.PHONY: build clean
default: run_main


build: $(BUILD_DIR)/Makefile
	cd $(BUILD_DIR) && make --trace

$(BUILD_DIR)/Makefile: Makefile
	mkdir -p $(BUILD_DIR) && cmake -S . -B $(BUILD_DIR) -DCMAKE_TOOLCHAIN_FILE=$(VCPKG_HOME)/scripts/buildsystems/vcpkg.cmake

clean:	
	rm -rf $(BUILD_DIR)


run_map_ll_ll: build
	$(BUILD_DIR)/map_ll_ll

run_map_ll_pair: build
	$(BUILD_DIR)/map_ll_pair

run_map_ll_array: build
	$(BUILD_DIR)/map_ll_array


run_map_person_int_lambda: build
	$(BUILD_DIR)/map_person_int_lambda

run_map_person_int_function_struct: build
	$(BUILD_DIR)/map_person_int_function_struct


run_map_pair_uint64: build
	$(BUILD_DIR)/map_pair_uint64
	
run_map_uint64_shared_ptr: build
	$(BUILD_DIR)/map_uint64_shared_ptr
