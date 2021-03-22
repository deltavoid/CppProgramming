

BUILD_DIR := build_release
.PHONY: run build clean

run: build
	$(BUILD_DIR)/main


build: $(BUILD_DIR)/Makefile
	cd $(BUILD_DIR) && make VERBOSE=1

$(BUILD_DIR)/Makefile:
	echo $(VCPKG_HOME)
	mkdir $(BUILD_DIR) && cmake -S . -B $(BUILD_DIR) -DCMAKE_TOOLCHAIN_FILE=$(VCPKG_HOME)/scripts/buildsystems/vcpkg.cmake

clean:
	rm -rf $(BUILD_DIR)