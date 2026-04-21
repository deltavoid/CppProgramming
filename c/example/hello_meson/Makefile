

BUILD_DIR ?= build
.PHONY: build clean run
default: run


build: $(BUILD_DIR)/build.ninja
	cd $(BUILD_DIR) && ninja

$(BUILD_DIR)/build.ninja: Makefile
	mkdir -p $(BUILD_DIR) \
	&& meson setup $(BUILD_DIR) 
	

clean:	
	rm -rf $(BUILD_DIR)


run: build
	$(BUILD_DIR)/main hello world

ldd: build
	ldd $(BUILD_DIR)/main
