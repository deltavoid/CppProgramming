cmake_minimum_required(VERSION 3.5)
# message("hello CMakeLists begin")

project(main)


message("CMAKE_PROJECT_NAME: ${CMAKE_PROJECT_NAME}")
message("CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
message("CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")

message("PROJECT_NAME: ${PROJECT_NAME}")
message("PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
message("PROJECT_BINARY_DIR: ${PROJECT_BINARY_DIR}")

message("CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
message("CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")
message("CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}")


find_package(fmt REQUIRED)



add_executable(main main.cpp)
target_link_libraries(main
    PRIVATE
        fmt::fmt
)


# message("hello CMakeLists end")
