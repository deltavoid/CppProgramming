cmake_minimum_required(VERSION 3.5)

project(thread_local)


add_executable(__thread __thread.c)
target_link_libraries(__thread PRIVATE -lpthread)

# add_executable(main1 main1.cpp)
# target_link_libraries(main1 PRIVATE -lpthread)

add_executable(thread_local thread_local.cpp)
target_link_libraries(thread_local PRIVATE -lpthread)

