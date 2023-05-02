cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

set(app_name PARSER_EMBARQUE)

project(${app_name} VERSION 0.1)

add_subdirectory(expression_parser)

add_executable(${app_name} parser_client/main.cpp)

target_include_directories(${app_name} PUBLIC ${CMAKE_SOURCE_DIR}/expression_parser/include)

target_link_libraries(${app_name} ${CMAKE_SOURCE_DIR}/build/expression_parser/libexpression_parser.a)

