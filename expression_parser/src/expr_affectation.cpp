#include "expr_affectation.hpp"

long Set_Expression::evaluate(const DataContext* dc) const
{
    if(left_member && right_member)
    {
        long val = right_member->evaluate(dc);
        return val;
    }
    return 0;
}

bool Set_Expression::update(DataContext* dc)
{
    if(left_member && right_member)
    {
        long val = right_member->evaluate(dc);
        return left_member->set(dc, val);
    }

    return false;
}

long Increase_Expression::evaluate(const DataContext* dc) const
{
    if(left_member && right_member)
    {
        long val = right_member->evaluate(dc);
        return left_member->evaluate(dc) + val;
    }
    return 0;
}

bool Increase_Expression::update(DataContext* dc)
{
    if(left_member && right_member)
    {
        long val = right_member->evaluate(dc);
        return left_member->increase(dc, val);
    }

    return false;
}

long Decrease_Expression::evaluate(const DataContext* dc) const
{
    if(left_member && right_member)
    {
        long val = right_member->evaluate(dc);
        return left_member->evaluate(dc) - val;
    }
    return 0;
}

bool Decrease_Expression::update(DataContext* dc)
{
    if(left_member && right_member)
    {
        long val = right_member->evaluate(dc);
        return left_member->decrease(dc, val);
    }

    return false;
}