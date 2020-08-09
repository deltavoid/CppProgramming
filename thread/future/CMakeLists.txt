cmake_minimum_required(VERSION 3.5)

project(future)


add_executable(main main.cpp)
target_link_libraries(main PRIVATE -lpthread)