cmake_minimum_required(VERSION 3.5)

project(eventloop)


add_executable(main main.cpp)
target_link_libraries(main PRIVATE -lpthread)

add_executable(main1 main1.cpp)
target_link_libraries(main1 PRIVATE -lpthread)