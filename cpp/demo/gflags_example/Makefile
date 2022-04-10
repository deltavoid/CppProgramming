

BUILD_DIR ?= build
.PHONY: build clean run
default: run


build: $(BUILD_DIR)/Makefile
	cd $(BUILD_DIR) && make --trace

$(BUILD_DIR)/Makefile: Makefile
	mkdir -p $(BUILD_DIR) \
	&& cmake -S . -B $(BUILD_DIR) 
	

clean:	
	rm -rf $(BUILD_DIR)


run: build
	$(BUILD_DIR)/main --echo_attachment true --timeout_ms 10 --server 127.0.0.1:8000

ldd: build
	ldd $(BUILD_DIR)/main
