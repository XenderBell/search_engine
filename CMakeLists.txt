cmake_minimum_required(VERSION 3.31)
project(search_engine)

set(CMAKE_CXX_STANDARD 20)

set(JSON_BuildTests OFF CACHE INTERNAL "")

add_subdirectory(nlohmann_json)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL
        https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

add_executable(search_engine src/main.cpp
        include/ConverterJSON.h
        src/ConverterJSON.cpp
        include/index.h
        include/launchExceptions.h
        include/textEdit.h
        src/textEdit.cpp
        include/InvertedIndex.h
        src/InvertedIndex.cpp
        include/SearchServer.h
        src/SearchServer.cpp)

target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json gtest_main)

include(GoogleTest)
gtest_discover_tests(search_engine)