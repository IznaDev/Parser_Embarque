#pragma once

#include "code_builder.hpp"
#include <set>
#include <algorithm>
#include <iostream>
#include <string>
#include "expression.hpp"

class ArduinoCodeBuilder : public ICodeBuilder
{

    public:
        void build(const json& json, const filesystem::path& output_directory) override;
};


class ArduinoCodebuilderExpressionVisitor: public IExpressionVisitor
{
    ostream& output;
    const string& expr_id;
    private:
        void add_member(const string& var_id, int& index, const Expression* expr);
        void add_member_ref(const string& ref_id, const Expression* expr);
    public:
        ArduinoCodebuilderExpressionVisitor(ostream& output, const string& epxr_id): output(output), expr_id(epxr_id){}
        void visit(const Expression* expr) override;
        void visit(const Constant_Expression* expr) override;
        void visit(const Reference_Expression* expr) override;
        void visit(const Binary_Operation_Expression* expr) override;
        void visit(const Affectation_Expression* expr) override;
};