#include "expr_functions.hpp"

long Max_Function_Expression::evaluate(const DataContext* dc) const
{
    long max_val = last_index == 0? 0 : args[0]->evaluate(dc);
    for(int i=1;i<last_index;i++)
    {
        long val = args[i]->evaluate(dc);
        if(val > max_val)
        {
            max_val = val;
        }
    }
    return max_val;
}

long Min_Function_Expression::evaluate(const DataContext* dc) const
{
    long min_val = last_index == 0? 0 : args[0]->evaluate(dc);
    for(int i=1;i<last_index;i++)
    {
        long val = args[i]->evaluate(dc);
        if(val < min_val)
        {
            min_val = val;
        }
    }
    return min_val;
}

long Pow_Function_Expression::evaluate(const DataContext* dc) const
{
    if(last_index>=2)
    {
        long x = args[0]->evaluate(dc);
        long y = args[1]->evaluate(dc);
        return pow(x,y);
    }
    else if(last_index==1)
    {
        return args[0]->evaluate(dc);
    }
    return 0;
}

long Sqrt_Function_Expression::evaluate(const DataContext* dc) const
{
    if(last_index>=1)
    {
        long x = args[0]->evaluate(dc);
        return sqrt(x);
    }
    return 0;
}

long Custom_Function_Expression::evaluate(const DataContext* dc) const
{
    MemoryDataContext mdc;
    int i=1;
    if(dc != nullptr)
    {
        int dc_size = dc->get_size();
        for (int i=0;i<dc_size;i++)
        {
            const char* key = dc->get_key(i);
            mdc.add_or_set(key, dc->evaluate(key));
        }
    }
    for(int i=0;i<last_index;i++)
    {
        mdc.add_or_set(var_args[i], args[i]->evaluate(dc));
    }
    return expression->evaluate(&mdc);
}