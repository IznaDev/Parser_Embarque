#pragma once

#include <string>
#include <vector>
#include <fstream>

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
        virtual void build(string json_file) = 0;
};