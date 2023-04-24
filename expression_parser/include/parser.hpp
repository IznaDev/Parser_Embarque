#pragma once

#include "expression.hpp"
#include "expr_mathematical.hpp"
#include "expr_logical.hpp"
#include "expr_functions.hpp"
#include "expr_affectation.hpp"
#include <string>
#include <stack>
#include <sstream>
#include <map>
#include <memory>
#include <functional>
#include <regex>
#include <iostream>
#include <queue>
#include "validation_handler.hpp"

using namespace std;

struct Parse_Result
{
    unique_ptr<Expression> expression{nullptr};
    string error_message{""};
    operator bool() const {if(expression){return true;} return false;};
};

class Expression_Parser
{
    private:
        map<string, function<unique_ptr<Operation_Expression>()>> operation_factory;
        map<string, function<unique_ptr<Function_Expression>()>> function_factory;
        Validation_Result validate(const string& expression) const noexcept;
        bool is_id(vector<string>& supported_operators, istringstream& stream, string& first_char) const;
        Parse_Result parse(istringstream& stream) const noexcept;
        vector<string> getSupportedOperatorsReg() const;
        string get_ops_regex_string(const string& expr) const;
        string get_func_regex_string(const string& expr) const;
        unique_ptr<Operation_Expression> build_operation(const string& id) const {return operation_factory.at(id)();}
        Parse_Result build_function(const string& id, istringstream& s) const noexcept;
        unique_ptr<ValidationHandler> validator;
    public:
        Expression_Parser();
        vector<string> getSupportedOperators() const;
        vector<string> getSupportedFunctions() const;
        Parse_Result parse(const string& expression_string) const noexcept;
        Validation_Result add_customFunction(const string& id, const string& expresssion);
    friend ExpressionValidationHandler;
    friend AffecationValidationHandler;
};