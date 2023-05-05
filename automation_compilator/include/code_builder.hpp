#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>

using json =  nlohmann::json;

using namespace std;

static vector<string> get_core_includes()
{
    vector<string> result;
    result.push_back("automation.hpp");
    return result;
}

void add_include(ostream& output, const string& include);

class ICodeBuilder
{
    public:
        virtual void build(const json& json) = 0;
};