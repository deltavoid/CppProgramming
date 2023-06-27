

BUILD_DIR ?= build
.PHONY: build clean run
default: run


build: $(BUILD_DIR)/Makefile
	cd $(BUILD_DIR) && make --trace

$(BUILD_DIR)/Makefile: Makefile
	mkdir -p $(BUILD_DIR) && cd $(BUILD_DIR) && cmake ..
	

clean:	
	rm -rf $(BUILD_DIR)


run: build
	$(BUILD_DIR)/main hello world

ldd: build
	ldd $(BUILD_DIR)/main
