#include "expression.hpp"
#include <math.h>

class Function_Expression: public Expression
{
    protected:
        static const int max_size=256;
        const char* id;
        int last_index{0};
        Expression* args[Function_Expression::max_size];
    public:
        Function_Expression(const char* id): id(id){};
        void add_arg(Expression* expr){
            if(last_index<Function_Expression::max_size)
            {
                args[last_index++] = expr;
            }
        }
};

class Max_Function_Expression: public Function_Expression
{
    public:
        Max_Function_Expression(): Function_Expression("max"){}
        long evaluate(const DataContext* dc) const override;

};

class Min_Function_Expression: public Function_Expression
{
    public:
        Min_Function_Expression(): Function_Expression("min"){}
        long evaluate(const DataContext* dc) const override;

};

class Pow_Function_Expression: public Function_Expression
{
    public:
        Pow_Function_Expression(): Function_Expression("pow"){}
        long evaluate(const DataContext* dc) const override;

};

class Sqrt_Function_Expression: public Function_Expression
{
    public:
        Sqrt_Function_Expression(): Function_Expression("sqrt"){}
        long evaluate(const DataContext* dc) const override;

};

class Custom_Function_Expression: public Function_Expression
{
    private: 
        Expression* expression;
        const char* var_args[Function_Expression::max_size]={"1","2","3","4","5","6", "7","8","9","10"};
    public:
        Custom_Function_Expression(const char* id, Expression* func): Function_Expression(id), expression(func){
            func = nullptr;
        }
        long evaluate(const DataContext* dc) const override;
};