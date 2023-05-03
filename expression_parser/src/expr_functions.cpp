#include "expr_functions.hpp"

long Max_Function_Expression::evaluate(const DataContext* dc) const
{
    vector<long> values;
    transform(begin(args), end(args), back_inserter(values),[dc](const unique_ptr<Expression>& expr){return expr->evaluate(dc);});
    long default_val = values.empty()? 0 : values[0];
    long result = accumulate(begin(values), end(values), default_val, [](long val_1, long val_2){return max(val_1, val_2);});
    return result;
    return 0;
}

long Min_Function_Expression::evaluate(const DataContext* dc) const
{
    vector<long> values;
    transform(begin(args), end(args), back_inserter(values),[dc](const unique_ptr<Expression>& expr){return expr->evaluate(dc);});
    long default_val = values.empty()? 0 : values[0];
    long result = accumulate(begin(values), end(values), default_val, [](long val_1, long val_2){return min(val_1, val_2);});
    return result;
    return 0;
}

long Pow_Function_Expression::evaluate(const DataContext* dc) const
{
    if(args.size()>=2)
    {
        long x = args[0]->evaluate(dc);
        long y = args[1]->evaluate(dc);
        return pow(x,y);
    }
    else if(args.size()==1)
    {
        return args[0]->evaluate(dc);
    }
    return 0;
}

long Sqrt_Function_Expression::evaluate(const DataContext* dc) const
{
    if(args.size()>=1)
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
    for(const auto& a: args)
    {
        mdc.add_or_set(std::to_string(i++).c_str(), a->evaluate(dc));
    }
    return expression->evaluate(&mdc);
}