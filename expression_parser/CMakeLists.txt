cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

set(app_name expression_parser)

project(${app_name} VERSION 0.1)



add_library(${app_name} STATIC 
    src/expression.cpp
    src/parser.cpp 
    src/data_context.cpp
    src/expr_mathematical.cpp
    src/expr_logical.cpp
    src/expr_functions.cpp
    src/validation_handler.cpp
    src/expr_affectation.cpp
    src/utils.cpp)
    

target_include_directories(${app_name} PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_subdirectory(tests)



