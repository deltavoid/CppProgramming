

BUILD_DIR ?= build
.PHONY: build clean
default: run_argv


build: $(BUILD_DIR)/Makefile
	cd $(BUILD_DIR) && make --trace

$(BUILD_DIR)/Makefile: Makefile
	mkdir -p $(BUILD_DIR) && cmake -S . -B $(BUILD_DIR) -DCMAKE_TOOLCHAIN_FILE=/data00/zhangqianyu/WorkSpace/vcpkg/scripts/buildsystems/vcpkg.cmake

clean:	
	rm -rf $(BUILD_DIR)


run_argv: build
	$(BUILD_DIR)/argv --hello world