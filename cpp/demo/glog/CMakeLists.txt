cmake_minimum_required(VERSION 3.5)
project(glog_demo)

message("CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}") 

find_package(glog CONFIG REQUIRED)

if (NOT glog_FOUND)
    message(FATAL_ERROR "glog Not Found!")
else()
    message("glog_INCLUDE_DIRS: " "${GLOG_INCLUDE_DIRS}" " done")
    message("glog_LIBRARIES: " "${GLOG_LIBS}")
endif()


add_executable(main 
    main.cpp
)

target_link_libraries(main 
    PRIVATE 
        glog::glog
)
