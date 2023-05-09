set(comp_name automation_code_builder)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

add_executable(${comp_name} main.cpp
    src/code_builder.cpp
    src/arduino_code_builder.cpp
    src/validator.cpp)

target_include_directories(${comp_name} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/expression_parser/include
    ${CMAKE_SOURCE_DIR}/json_schema_validator/src)

target_link_libraries(${comp_name} 
PUBLIC 
    ${CMAKE_SOURCE_DIR}/build/expression_parser/src/libexpression_parser.a
PRIVATE 
    nlohmann_json::nlohmann_json
PRIVATE
    nlohmann_json_schema_validator)

add_subdirectory(tests_automation)