cmake_minimum_required(VERSION 3.31)

project(server_test)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL
        https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(server_test index_tests.cpp server_tests.cpp
        ${CMAKE_SOURCE_DIR}/include/InvertedIndex.h
        ${CMAKE_SOURCE_DIR}/include/SearchServer.h
        ${CMAKE_SOURCE_DIR}/src/InvertedIndex.cpp
        ${CMAKE_SOURCE_DIR}/src/SearchServer.cpp
        ${CMAKE_SOURCE_DIR}/src/textEdit.cpp)

target_include_directories(server_test PUBLIC ${gtest_SOURCE_DIR}/include ${INCLUDE_DIR})

target_link_libraries(server_test PRIVATE gtest_main)
