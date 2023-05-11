include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
set(test_comp_name automation_code_builder_test)

enable_testing()

include_directories(${CMAKE_SOURCE_DIR}/automation_code_builder/include)

add_executable(${test_comp_name}
model/test_jsonValid.cpp
${CMAKE_SOURCE_DIR}/automation_code_builder/src/validator.cpp)

target_link_libraries(${test_comp_name} 
PUBLIC
    GTest::gtest_main
  PRIVATE 
    nlohmann_json::nlohmann_json
PRIVATE
    nlohmann_json_schema_validator
)

include(GoogleTest)
gtest_discover_tests(automation_code_builder_test)