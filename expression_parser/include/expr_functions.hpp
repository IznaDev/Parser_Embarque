#pragma once
#include "expression.hpp"
#include <math.h>


class Max_Function_Expression: public Function_Expression
{
    public:
        Max_Function_Expression(): Function_Expression("max"){}
        long evaluate(const DataContext* dc) const override;
        const char* to_cstr() const override {return "Max_Function_Expression";}

};

class Min_Function_Expression: public Function_Expression
{
    public:
        Min_Function_Expression(): Function_Expression("min"){}
        long evaluate(const DataContext* dc) const override;
        const char* to_cstr() const override {return "Min_Function_Expression";}

};

class Pow_Function_Expression: public Function_Expression
{
    public:
        Pow_Function_Expression(): Function_Expression("pow"){}
        long evaluate(const DataContext* dc) const override;
        const char* to_cstr() const override {return "Pow_Function_Expression";}

};

class Sqrt_Function_Expression: public Function_Expression
{
    public:
        Sqrt_Function_Expression(): Function_Expression("sqrt"){}
        long evaluate(const DataContext* dc) const override;
        const char* to_cstr() const override {return "Sqrt_Function_Expression";}

};

class Custom_Function_Expression: public Function_Expression
{
    private: 
        Expression* expression;
        const char* var_args[Function_Expression::max_size]={"1","2","3","4","5","6", "7","8","9","10"};
    protected:
        void clean() override 
        {
            if(expression)
            {
                delete expression;
                expression = nullptr;
            }
        }
    public:
        Custom_Function_Expression(const char* id, Expression* func): Function_Expression(id), expression(func){
            func = nullptr;
        }
        long evaluate(const DataContext* dc) const override;
        const char* to_cstr() const override {return "Custom_Function_Expression";}
};