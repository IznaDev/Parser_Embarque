set(client_name parser_client)

add_executable(${client_name} main.cpp)

target_include_directories(${client_name} PUBLIC ${CMAKE_SOURCE_DIR}/expression_parser/include)

target_link_libraries(${client_name} ${PROJECT_LIBS_PUBLIC})