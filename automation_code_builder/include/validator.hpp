#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>
#include <nlohmann/json-schema.hpp>

using namespace std;
using json = nlohmann::json;
using json_validator = nlohmann::json_schema::json_validator;

json createSchema(string schema);

json_validator validSchema(string schema);

void validJson(string schema, json file);
