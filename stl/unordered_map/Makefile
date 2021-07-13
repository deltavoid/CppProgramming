

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