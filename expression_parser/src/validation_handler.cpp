#include "validation_handler.hpp"


Validation_Result EmptyValidationHandler::validate(const string& expr) const noexcept
{
    // Empty or white space
    if(is_whitespace_or_empty(expr))
    {
        return Validation_Result{false,"Expression should not be empty"};
    }

    return validate_next(expr);
}

Validation_Result ParenthesisValidationHandler::validate(const string& expr) const noexcept
{
    // Unbalanced parenthesis
    int nb_open_parenthesis = count_if(begin(expr), end(expr), [](const char c){return c == '(';});
    int nb_close_parenthesis = count_if(begin(expr), end(expr), [](const char c){return c == ')';});
    if(nb_open_parenthesis != nb_close_parenthesis)
    {
        ostringstream s;
        s << "The number of opening parenthesis (" << nb_open_parenthesis << ") is not equal to the number of closing parenthesis (" << nb_close_parenthesis << ")";
        return Validation_Result{false, s.str()};
    }

    
    int openingIndex = -1;
    string substr = expr;
    while(nb_close_parenthesis > 0)
    {
        auto closing_it=substr.begin();
        int last_closing_index = substr.find_last_of(')');
        advance(closing_it, last_closing_index);
        nb_open_parenthesis = count(substr.begin(),closing_it,'(');
        if (nb_open_parenthesis<nb_close_parenthesis)
        {
            return Validation_Result{false,"The closing parenthesis cannot be before the opening parenthesis"};
        }
        substr = substr.substr(0,last_closing_index);
        nb_close_parenthesis--;
    }

    return validate_next(expr);
}