

BUILD_DIR ?= build
.PHONY: build clean
default: run_template_function


build: $(BUILD_DIR)/Makefile
	cd $(BUILD_DIR) && make --trace

$(BUILD_DIR)/Makefile: Makefile
	mkdir -p $(BUILD_DIR) && cmake -S . -B $(BUILD_DIR) -DCMAKE_TOOLCHAIN_FILE=/data00/zhangqianyu/WorkSpace/vcpkg/scripts/buildsystems/vcpkg.cmake

clean:	
	rm -rf $(BUILD_DIR)


run_template_function: build
	$(BUILD_DIR)/template_function

run_template_class: build
	$(BUILD_DIR)/template_class

run_template_class_template_function: build
	$(BUILD_DIR)/template_class_template_function

