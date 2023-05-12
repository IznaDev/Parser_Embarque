#pragma once
#include "code_builder.hpp"
#include "arduino_code_builder.hpp"
#include <functional>


namespace CodeBuilderFactory
{
    const map<string, function<ICodeBuilder*()>> code_builders{{"arduino", [](){ return new ArduinoCodeBuilder();}}};
    static ICodeBuilder* build(const string& target)
    {
        return code_builders.at(target)();
    }
};