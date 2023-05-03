cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(app_name json_validator)

project(${app_name} VERSION 0.1)

add_library(${app_name} STATIC
    src/validator.cpp
)

include_directories(../../json-schema-validator/build/)

link_directories(../../json-schema-validator/build/json-schema-validate)

find_package(nlohmann_json 3.9.1 REQUIRED)

add_executable(${app_name} src/main.cpp src/validator.cpp include/validator.hpp)

target_link_libraries(${app_name} PRIVATE nlohmann_json::nlohmann_json nlohmann_json_schema_validator)

add_subdirectory(tests)