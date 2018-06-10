cmake_minimum_required(VERSION 3.11)
project(pattern)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-fsanitize=address)

set(GTEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/googletest/googletest)
include_directories(${GTEST_DIR})
include_directories(${GTEST_DIR}/src)
include_directories(${GTEST_DIR}/include)

add_executable(pattern observable_test.cpp observable.h ${GTEST_DIR}/src/gtest-all.cc ${GTEST_DIR}/src/gtest_main.cc)
target_link_libraries(pattern pthread rt)
target_link_libraries(pattern asan)