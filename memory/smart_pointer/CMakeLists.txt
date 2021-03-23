cmake_minimum_required(VERSION 3.5)

project(smart_pointer)


find_path(BOOST_SMART_PTR_INCLUDE_DIRS "boost/detail/atomic_count.hpp")
    

# add_executable(scoped_ptr scoped_ptr.cpp)
# target_include_directories(scoped_ptr PRIVATE ${BOOST_SMART_PTR_INCLUDE_DIRS})


# add_executable(unique_ptr1 unique_ptr1.cpp)
# add_executable(unique_ptr2 unique_ptr2.cpp)
# add_executable(shared_ptr1 shared_ptr1.cpp)
# add_executable(shared_ptr2 shared_ptr2.cpp)


add_subdirectory(scoped_ptr)
add_subdirectory(unique_ptr)
add_subdirectory(shared_ptr)
