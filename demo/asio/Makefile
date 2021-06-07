

BUILD_DIR?=build
.PHONY: build clean run
# default: run_server


build: $(BUILD_DIR)
	cd $(BUILD_DIR) && make 

$(BUILD_DIR)/Makefile:
	mkdir -p $(BUILD_DIR) \
	&& cmake -S . -B $(BUILD_DIR) \
	    -DCMAKE_TOOLCHAIN_FILE=$(VCPKG_HOME)/scripts/buildsystems/vcpkg.cmake

clean:
	rm -rf $(BUILD_DIR)


# run: build
# 	$(BUILD_DIR)/main
run_server: build
	$(BUILD_DIR)/cpp11/echo/async_tcp_echo_server 8192

run_client: build
	$(BUILD_DIR)/cpp11/echo/blocking_tcp_echo_client 127.0.0.1 8192 
