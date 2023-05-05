cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

set(app_name expression_parser)

project(${app_name} VERSION 0.1)

add_subdirectory(src)

add_subdirectory(tests)



