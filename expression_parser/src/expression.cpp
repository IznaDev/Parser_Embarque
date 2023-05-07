#include "expression.hpp"

bool is_whitespace_or_empty(const char* text)
{
    for(int i=0;i<strlen(text);i++)
    {
        if(text[i] != ' ' && text[i] != '\t') return false;
    }

    return false;
}

void Binary_Operation_Expression::accept(IExpressionVisitor* visitor) const
{
    if(visitor)
    {
        visitor->visit(this);
    }
}

void Constant_Expression::accept(IExpressionVisitor* visitor) const
{
    if(visitor)
    {
        visitor->visit(this);
    }
}

void Reference_Expression::accept(IExpressionVisitor* visitor) const
{
    if(visitor)
    {
        visitor->visit(this);
    }
}

void Affectation_Expression::accept(IExpressionVisitor* visitor) const
{
    if(visitor)
    {
        visitor->visit(this);
    }
}

void Unary_Operation_Expression::accept(IExpressionVisitor* visitor) const
{
    if(visitor)
    {
        visitor->visit(this);
    }
}

void Function_Expression::accept(IExpressionVisitor* visitor) const
{
    if(visitor)
    {
        visitor->visit(this);
    }
}

void Custom_Function_Expression::accept(IExpressionVisitor* visitor) const
{
    if(visitor)
    {
        visitor->visit(this);
    }
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