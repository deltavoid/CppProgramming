

BUILD_DIR := build_release

run: build
	$(BUILD_DIR)/main img.jpg

build: $(BUILD_DIR)
	cd $(BUILD_DIR) && make VERBOSE=1

$(BUILD_DIR):
	mkdir $(BUILD_DIR) && cmake -S . -B $(BUILD_DIR) -DCMAKE_TOOLCHAIN_FILE=/mnt/c/Users/ZhangQianyu/WorkSpace/vcpkg/scripts/buildsystems/vcpkg.cmake

clean:
	rm -rf $(BUILD_DIR)