cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(Heaps)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(lib/googletest-master)
include_directories(lib/googletest-master/googletest/include)
include_directories(lib/googletest-master/googlemock/include)


add_executable(Heaps main.cpp src/BinomialHeap/BinomialHeap.h Tests/BinomialHeapTests/test.cpp)

target_link_libraries(Heaps gtest gtest_main)