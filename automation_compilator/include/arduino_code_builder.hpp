#pragma once

#include "code_builder.hpp"
#include <set>
#include <algorithm>
#include <iostream>
#include "expression.hpp"

class ArduinoCodeBuilder : public ICodeBuilder
{

    public:
        void build(const json& json) override;
};


class ArduinoCodebuilderExpressionVisitor: public IExpressionVisitor
{
    public:
        void visit(const Expression* expr) override;
        void visit(const Binary_Operation_Expression* expr) override;
};