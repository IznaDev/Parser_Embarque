cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

set(proj_name parser_embarque)

project(${proj_name} VERSION 0.1)

include_directories(../../json-schema-validator/build/)
link_directories(../../json-schema-validator/build/json-schema-validate)

find_package(nlohmann_json 3.9.1 REQUIRED)

add_subdirectory(expression_parser)

add_subdirectory(parser_client)

add_subdirectory(automation_compilator)

