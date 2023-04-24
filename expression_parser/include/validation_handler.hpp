#pragma once

#include <string>
#include <memory>
#include <algorithm>
#include <sstream>
#include "utils.hpp"
#include <regex>

using namespace std;

struct Validation_Result
{
    bool is_valid{true};
    string message{""};
    operator bool() const {return is_valid;}
};

class Expression_Parser;

class ValidationHandler
{
    private:
        unique_ptr<ValidationHandler> next{nullptr};
    protected:
        Validation_Result validate_next(const string& expr) const noexcept
        {       
            if(next)
            {
                return next->validate(expr);
            }
            return Validation_Result();
        }

    public:
        virtual Validation_Result validate(const string& expr) const noexcept = 0;
        void set_next(unique_ptr<ValidationHandler>&& next){this->next = move(next);}
};

class EmptyValidationHandler : public ValidationHandler
{
    Validation_Result validate(const string& expr) const noexcept override;
};

class ParenthesisValidationHandler : public ValidationHandler
{
    Validation_Result validate(const string& expr) const noexcept override;
};

class ExpressionValidationHandler: public ValidationHandler
{
    private:
        const Expression_Parser& parser;
    public:
    ExpressionValidationHandler(const Expression_Parser& parser): parser(parser){};
    Validation_Result validate(const string& expr) const noexcept override;
};

class AffecationValidationHandler: public ValidationHandler
{
    private:
        const Expression_Parser& parser;
    public:
    AffecationValidationHandler(const Expression_Parser& parser): parser(parser){};
    Validation_Result validate(const string& expr) const noexcept override;
};