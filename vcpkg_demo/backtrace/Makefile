

BUILD_DIR := build_release
.PHONY: run build clean

run: build
	$(BUILD_DIR)/main


build: $(BUILD_DIR)/Makefile
	cd $(BUILD_DIR) && make --trace

$(BUILD_DIR)/Makefile:
# 	mkdir -p $(BUILD_DIR) && cmake -S . -B $(BUILD_DIR) -DCMAKE_TOOLCHAIN_FILE=/mnt/c/Users/ZhangQianyu/WorkSpace/vcpkg/scripts/buildsystems/vcpkg.cmake
	mkdir -p $(BUILD_DIR) && cmake -S . -B $(BUILD_DIR) -DCMAKE_TOOLCHAIN_FILE=/home/zqy/WorkSpace/vcpkg/scripts/buildsystems/vcpkg.cmake
# 	mkdir -p $(BUILD_DIR) && cmake -S . -B $(BUILD_DIR) -DCMAKE_TOOLCHAIN_FILE=$(VCPKG_HOME)/scripts/buildsystems/vcpkg.cmake



clean:
	rm -rf $(BUILD_DIR)


