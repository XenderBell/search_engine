add_subdirectory(../nlohmann_json ${CMAKE_CURRENT_BINARY_DIR}/nlohmann_json)

add_executable(search_engine
        main.cpp
        ConverterJSON.cpp
        InvertedIndex.cpp
        SearchServer.cpp
        textEdit.cpp
)

target_include_directories(search_engine PUBLIC ${INCLUDE_DIR})
target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)
