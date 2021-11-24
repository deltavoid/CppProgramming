cmake_minimum_required(VERSION 3.5)

project(pthread)


add_executable(pthread pthread.cpp)
target_link_libraries(pthread PRIVATE -lpthread)

add_executable(condition_variable condition_variable.cpp)
target_link_libraries(condition_variable PRIVATE -lpthread)

add_executable(bounded_blocking_queue bounded_blocking_queue.cpp)
target_link_libraries(bounded_blocking_queue PRIVATE -lpthread)