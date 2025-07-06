cmake_minimum_required(VERSION 3.31)
project(search_engine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(tests)
add_subdirectory(src)
