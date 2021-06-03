cmake_minimum_required(VERSION 3.5)
project(asio_demo)


find_package(glog CONFIG REQUIRED)
find_package(asio CONFIG REQUIRED)

if (NOT asio_FOUND)
    message(FATAL_ERROR "asio Not Found!")
else()
    message("asio_INCLUDE_DIRS: " "${ASIO_INCLUDE_DIR}" " done")
    message("asio_LIBRARIES: " "${ASIO_LIBS}")
endif()

add_subdirectory(cpp11)
#命名冲突？
#add_subdirectory(cpp14)
#add_subdirectory(cpp17)