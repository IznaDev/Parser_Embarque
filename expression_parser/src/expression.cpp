#include "expression.hpp"


long Constant_Expression::evaluate(const DataContext* dc) const
{
    if(is_whitespace_or_empty(this->value)) return 0;
    return stol(this->value);
}

long Reference_Expression::evaluate(const DataContext* dc) const
{
    if(dc && dc->exists(reference.c_str()))
    {
        return dc->evaluate(reference.c_str());
    }
    return 0;
}