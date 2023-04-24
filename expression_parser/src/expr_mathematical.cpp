#include "expr_mathematical.hpp"

long Addition_Expression::evaluate(const DataContext* dc) const
{
    if(left_member && right_member)
    {
        return left_member->evaluate(dc) + right_member->evaluate(dc);
    }
    return 0;
}

long Substraction_Expression::evaluate(const DataContext* dc) const
{
    if(left_member && right_member)
    {
        return left_member->evaluate(dc) - right_member->evaluate(dc);
    }
    return 0;
}

long Multiplication_Expression::evaluate(const DataContext* dc) const
{
    if(left_member && right_member)
    {
        return left_member->evaluate(dc) * right_member->evaluate(dc);
    }
    return 0;
}

long Division_Expression::evaluate(const DataContext* dc) const
{
    if(left_member && right_member)
    {
        int div = right_member->evaluate(dc);
        return left_member->evaluate(dc) / (div == 0 ? 1: div);
    }
    return 0;
}

long Modulo_Expression::evaluate(const DataContext* dc) const
{
    if(left_member && right_member)
    {
         int div = right_member->evaluate(dc);
        return left_member->evaluate(dc)% (div == 0 ? 1 : div);
    }
    return 0;
}