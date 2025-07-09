add_subdirectory(../nlohmann_json ${CMAKE_CURRENT_BINARY_DIR}/nlohmann_json)

add_executable(search_engine
        main.cpp
        ConverterJSON.cpp
        InvertedIndex.cpp
        SearchServer.cpp
        textEdit.cpp
)

configure_file(${CMAKE_SOURCE_DIR}/config/config.json ${CMAKE_CURRENT_BINARY_DIR})
configure_file(${CMAKE_SOURCE_DIR}/config/requests.json ${CMAKE_CURRENT_BINARY_DIR})
configure_file(${CMAKE_SOURCE_DIR}/config/answers.json ${CMAKE_CURRENT_BINARY_DIR})

target_include_directories(search_engine PUBLIC ${INCLUDE_DIR})
target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)
