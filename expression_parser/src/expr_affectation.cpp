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

string Set_Expression::to_string() const
{
    stringstream s;
    s << left_member->to_string() << " is set to " << right_member->to_string();
    return s.str();
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

string Increase_Expression::to_string() const
{
    stringstream s;
    s << left_member->to_string() << " is increased by " << right_member->to_string();
    return s.str();
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

string Decrease_Expression::to_string() const
{
    stringstream s;
    s << left_member->to_string() << " is decreased by " << right_member->to_string();
    return s.str();
}