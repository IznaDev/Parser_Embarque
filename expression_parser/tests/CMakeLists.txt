include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
set(test_app_name expression_parser_tests)

enable_testing()



add_executable(${test_app_name} model/addition.test.cpp
  model/invalid.test.cpp
  model/substraction.test.cpp
  model/combined.test.cpp
  model/parenthesis.test.cpp
  model/multiplication.test.cpp
  model/division.test.cpp
  model/exp.test.cpp
  model/modulo.test.cpp
  model/equals.test.cpp
  model/lessthan.test.cpp
  model/greaterthan.test.cpp
  model/andor.test.cpp
  model/reference.test.cpp
  model/notoperator.test.cpp
  model/shift.test.cpp
  model/functions.test.cpp
  model/bitwise_logical.test.cpp
  model/affectation.test.cpp)

target_link_libraries(${test_app_name} PUBLIC model misc GTest::gtest_main)

target_include_directories(${test_app_name} PUBLIC include/model include/misc)

include(GoogleTest)
gtest_discover_tests(expression_parser_tests)