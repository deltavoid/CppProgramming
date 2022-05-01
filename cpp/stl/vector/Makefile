

BUILD_DIR ?= build
.PHONY: build clean
default: run


build: $(BUILD_DIR)/Makefile
	cd $(BUILD_DIR) && make --trace

$(BUILD_DIR)/Makefile: Makefile
	mkdir -p $(BUILD_DIR) && cmake -S . -B $(BUILD_DIR) 

clean:	
	rm -rf $(BUILD_DIR)


run: build
	$(BUILD_DIR)/main

run_shrink_to_fit: build
	$(BUILD_DIR)/shrink_to_fit