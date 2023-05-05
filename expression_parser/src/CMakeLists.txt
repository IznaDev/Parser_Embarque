set(lib_name expression_parser)

add_library(${lib_name} STATIC 
    parser.cpp 
    data_context.cpp
    validation_handler.cpp
    container.cpp
    utils.cpp
    expression.cpp
    expr_mathematical.cpp
    expr_logical.cpp
    expr_functions.cpp
    expr_affectation.cpp
)
    

target_include_directories(${lib_name} PUBLIC ${PROJECT_SOURCE_DIR}/include)