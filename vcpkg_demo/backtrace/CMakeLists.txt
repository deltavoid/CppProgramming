cmake_minimum_required(VERSION 3.5)
project(glog_demo)


find_package(glog CONFIG REQUIRED)

find_path(BOOST_STACKTRACE_INCLUDE_DIRS "boost/stacktrace.hpp")

#if (NOT glog_FOUND)
#    message(FATAL_ERROR "glog Not Found!")
#else()
#    message("glog_INCLUDE_DIRS: " "${GLOG_INCLUDE_DIRS}" " done")
#    message("glog_LIBRARIES: " "${GLOG_LIBS}")
#endif()


add_executable(main 
    main.cpp
)
target_compile_options(main
    PRIVATE
        -g
        -DBOOST_STACKTRACE_USE_ADDR2LINE
)
target_include_directories(main 
    PRIVATE 
        ${BOOST_STACKTRACE_INCLUDE_DIRS}
)
target_link_libraries(main 
    PRIVATE 
        glog::glog
        -ldl
        -lrt
        -rdynamic
)
