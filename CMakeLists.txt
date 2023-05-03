cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

set(proj_name parser_embarque)
set(comp_name automation_compilator)
set(client_name parser_client)

project(${proj_name} VERSION 0.1)

add_subdirectory(expression_parser)

add_executable(${client_name} parser_client/main.cpp)

target_include_directories(${client_name} PUBLIC ${CMAKE_SOURCE_DIR}/expression_parser/include)

target_link_libraries(${client_name} ${CMAKE_SOURCE_DIR}/build/expression_parser/libexpression_parser.a)

add_executable(${comp_name} automation_compilator/main.cpp)

target_include_directories(${comp_name} PUBLIC ${CMAKE_SOURCE_DIR}/expression_parser/include)

target_link_libraries(${comp_name} ${CMAKE_SOURCE_DIR}/build/expression_parser/libexpression_parser.a)

