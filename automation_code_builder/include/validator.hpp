#pragma once

#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <nlohmann/json-schema.hpp>

using json = nlohmann::json;
using json_validator = nlohmann::json_schema::json_validator;
using namespace std;

json createSchema(string schema);
json_validator validSchema(string schema);
void validJson(string schema, json file);
