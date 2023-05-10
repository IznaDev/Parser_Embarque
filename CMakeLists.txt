cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

set(proj_name parser_embarque)

project(${proj_name} VERSION 0.1)

add_subdirectory(expression_parser)

add_subdirectory(parser_client)

add_subdirectory(automation_code_builder)

