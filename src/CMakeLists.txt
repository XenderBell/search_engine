include(FetchContent)
FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/nlohmann/json
        GIT_TAG v3.10.5
        )
FetchContent_MakeAvailable(json)

add_executable(search_engine
        main.cpp
        ConverterJSON.cpp
        InvertedIndex.cpp
        SearchServer.cpp
        textEdit.cpp
)

target_include_directories(search_engine PUBLIC ${INCLUDE_DIR})
target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)
