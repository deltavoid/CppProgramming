cmake_minimum_required(VERSION 3.0)
project( DisplayImage )

find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(main DisplayImage.cpp )
target_link_libraries(main ${OpenCV_LIBS} )


