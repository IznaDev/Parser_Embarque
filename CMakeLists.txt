cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

set(proj_name parser_embarque)

project(${proj_name} VERSION 0.1)

include(FetchContent)

FetchContent_Declare(
    json 
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

add_subdirectory(expression_parser)

add_subdirectory(parser_client)

add_subdirectory(automation_code_builder)

#Problème:
#En partant d'un projet sans dossier build
#1er build:
# Le projet est configuré, mais ne parvient pas à gérer la bibliothèque jscon_schema_validator
# La création du projet s'arrête

#On commente la ligne 3 du CMakeLists dans "automation_code_builder"
#2ème build:
# Crée tous les tests de "expression_parser"
# Parvient à crée "validator.cpp", ce qui signifie que la bibliothèque json_schema_validator est bien reconnue

#On décommente la ligne 3 du CMakeLists dans "automation_code_builder"
#3ème build:
# Créer l'intégralité des executables
