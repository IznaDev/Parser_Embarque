set(comp_name automation_code_builder)

add_subdirectory(json-schema-validator)

add_executable(${comp_name} main.cpp
    src/code_builder.cpp
    src/arduino_code_builder.cpp
    src/validator.cpp
    src/driver.cpp)

target_include_directories(${comp_name} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/json-schema-validator/src
    ${CMAKE_SOURCE_DIR}/expression_parser/include)

target_link_libraries(${comp_name} 
PUBLIC 
    ${PROJECT_LIBS_PUBLIC}
PRIVATE 
    nlohmann_json::nlohmann_json
    nlohmann_json_schema_validator)