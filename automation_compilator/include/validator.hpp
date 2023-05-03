#pragma once

#include <fstream>
#include <iostream>

#include <nlohmann/json.hpp>
#include <nlohmann/json-schema.hpp>

using namespace std;
using json = nlohmann::json;
using nlohmann::json_schema::json_validator;

bool isExist(string filepath);
bool isCorrect(string filepath);
json createSchema(string schema);
json_validator validSchema(string schema);
void validJson(string schema, string filepath);
