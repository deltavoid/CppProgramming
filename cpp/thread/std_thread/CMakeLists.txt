cmake_minimum_required(VERSION 3.5)

project(std_thread)


add_executable(blocking_queue blocking_queue.cpp)
target_link_libraries(blocking_queue PRIVATE -lpthread)

add_executable(condition_variable condition_variable.cpp)
target_link_libraries(condition_variable PRIVATE -lpthread)

add_executable(object_thread1 object_thread1.cpp) 
target_link_libraries(object_thread1 PRIVATE -lpthread)

add_executable(object_thread2 object_thread2.cpp)
target_link_libraries(object_thread2 PRIVATE -lpthread)