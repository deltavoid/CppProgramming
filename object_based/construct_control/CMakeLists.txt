cmake_minimum_required(VERSION 3.5)

project(main)
message("project ${CMAKE_PROJECT_NAME} begin")

message("CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
message("CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")
message("CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}")


add_executable(move_constructor move_constructor.cpp)

add_executable(copy_constructor copy_constructor.cpp)



message("project ${CMAKE_PROJECT_NAME} end")
