cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

set(proj_name parser_embarque)

project(${proj_name} VERSION 0.1)

include(FetchContent)

FetchContent_Declare(
    json
    DOWNLOAD_EXTRACT_TIMESTAMP ON
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

add_subdirectory(expression_parser)

set(PROJECT_LIBS ${PROJECT_LIBS}
    "expression_parser"
)

add_subdirectory(parser_client)

add_subdirectory(automation_code_builder)