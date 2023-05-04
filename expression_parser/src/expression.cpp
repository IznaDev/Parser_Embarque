#include "expression.hpp"

bool is_whitespace_or_empty(const char* text)
{
    for(int i=0;i<strlen(text);i++)
    {
        if(text[i] != ' ' && text[i] != '\t') return false;
    }

    return false;
}

long Constant_Expression::evaluate(const DataContext* dc) const
{
    return value;
}

long Reference_Expression::evaluate(const DataContext* dc) const
{
    if(dc && dc->exists(reference))
    {
        return dc->evaluate(reference);
    }
    return 0;
}