#include "expr_logical.hpp"


long Equals_Expression::evaluate(const DataContext* dc) const
{
    if(left_member && right_member)
    {
        return left_member->evaluate(dc) == right_member->evaluate(dc);
    }
    return 0;
}

long NotEquals_Expression::evaluate(const DataContext* dc) const
{
    if(left_member && right_member)
    {
        return left_member->evaluate(dc) != right_member->evaluate(dc);
    }
    return 0;
}

long LessThan_Expression::evaluate(const DataContext* dc) const
{
    if(left_member && right_member)
    {
        return left_member->evaluate(dc) < right_member->evaluate(dc);
    }
    return 0;
}

long LessThanEqual_Expression::evaluate(const DataContext* dc) const
{
    if(left_member && right_member)
    {
        return left_member->evaluate(dc) <= right_member->evaluate(dc);
    }
    return 0;
}

long GreaterThan_Expression::evaluate(const DataContext* dc) const
{
    if(left_member && right_member)
    {
        return left_member->evaluate(dc) > right_member->evaluate(dc);
    }
    return 0;
}

long GreaterThanEqual_Expression::evaluate(const DataContext* dc) const
{
    if(left_member && right_member)
    {
        return left_member->evaluate(dc) >= right_member->evaluate(dc);
    }
    return 0;
}

long And_Expression::evaluate(const DataContext* dc) const
{
    if(left_member && right_member)
    {
        return left_member->evaluate(dc) && right_member->evaluate(dc);
    }
    return 0;
}

long Or_Expression::evaluate(const DataContext* dc)const
{
    if(left_member && right_member)
    {
        return left_member->evaluate(dc) || right_member->evaluate(dc);
    }
    return 0;
}

long Not_Expression::evaluate(const DataContext* dc) const
{
    if(right_member)
    {
        return !right_member->evaluate(dc);
    }
    return 0;
}

long Shift_Left_Expression::evaluate(const DataContext* dc) const
{
    if(left_member && right_member)
    {
        return left_member->evaluate(dc) << right_member->evaluate(dc);
    }
    return 0;
}

long Shift_Right_Expression::evaluate(const DataContext* dc) const
{
    if(left_member && right_member)
    {
        return left_member->evaluate(dc) >> right_member->evaluate(dc);
    }
    return 0;
}

long XOR_Bitwise_Expression::evaluate(const DataContext* dc) const
{
    if(left_member && right_member)
    {
        return left_member->evaluate(dc) ^ right_member->evaluate(dc);
    }
    return 0;
}

long And_Bitwise_Expression::evaluate(const DataContext* dc) const
{
    if(left_member && right_member)
    {
        return left_member->evaluate(dc) & right_member->evaluate(dc);
    }
    return 0;
}

long Or_Bitwise_Expression::evaluate(const DataContext* dc) const
{
    if(left_member && right_member)
    {
        return left_member->evaluate(dc) | right_member->evaluate(dc);
    }
    return 0;
}