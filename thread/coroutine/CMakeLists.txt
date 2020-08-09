cmake_minimum_required(VERSION 3.5)

project(future)


find_package(Boost 
    COMPONENTS 
        program_options 
        system 
        coroutine 
        thread 
    REQUIRED)

if(Boost_FOUND)
    #include_directories(${Boost_INCLUDE_DIRS})
    
    MESSAGE("Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}.")
    MESSAGE("Boost_LIBRARIES = ${Boost_LIBRARIES}.")
    MESSAGE("Boost_LIB_VERSION = ${Boost_LIB_VERSION}.")
endif()



add_executable(main1 main1.cpp)
target_include_directories(main1
    PRIVATE
        ${Boost_INCLUDE_DIRS}
)
target_link_libraries(main1
    PRIVATE
        -lpthread
        ${Boost_LIBRARIES}
)


add_executable(main2 main2.cpp)
target_include_directories(main2
    PRIVATE
        ${Boost_INCLUDE_DIRS}
)
target_link_libraries(main2
    PRIVATE
        -lpthread
        ${Boost_LIBRARIES}
)


add_executable(ref1 ref1.cpp)
target_include_directories(ref1
    PRIVATE
        ${Boost_INCLUDE_DIRS}
)
target_link_libraries(ref1
    PRIVATE
        -lpthread
        ${Boost_LIBRARIES}
)


add_executable(ref2 ref2.cpp)
target_include_directories(ref2
    PRIVATE
        ${Boost_INCLUDE_DIRS}
)
target_link_libraries(ref2
    PRIVATE
        -lpthread
        ${Boost_LIBRARIES}
)


add_executable(ref3 ref3.cpp)
target_include_directories(ref3
    PRIVATE
        ${Boost_INCLUDE_DIRS}
)
target_link_libraries(ref3
    PRIVATE
        -lpthread
        ${Boost_LIBRARIES}
)