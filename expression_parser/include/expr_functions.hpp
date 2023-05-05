#pragma once
#include "expression.hpp"
#include <math.h>

class Function_Expression: public Expression
{
    protected:
        static const int max_size=256;
        const char* id;
        int last_index{0};
        Expression* args[Function_Expression::max_size];
        void clean() override
        {
            for(int i=0;i<last_index;i++)
            {
                if(args[i])
                {
                    delete args[i];
                    args[i]=nullptr;
                }
            }
            last_index = 0;
        }
    public:
        Function_Expression(const char* id): id(id){};
        void add_arg(Expression* expr){
            if(last_index<Function_Expression::max_size)
            {
                args[last_index++] = expr;
            }
        }
        Expression* simplify() override
        {
            for(int i=0;i<last_index;i++)
            {
                if(args[i])
                {
                    if(!args[i]->is_leaf())
                    {
                        Expression* expr = args[i]->simplify();
                        if(expr != args[i])
                        {
                            delete args[i];
                            args[i] = expr;
                        }
                    }
                }
            }
            if(!has_variable())
            {
                return new Constant_Expression(this->evaluate());
            }
            return this;
        }
        virtual bool has_variable() override
        {
            for(int i=0;i<last_index;i++)
            {
                if(args[i] && args[i]->has_variable())
                {
                    return true;
                }
            }

            return false;
        }
        virtual bool is_leaf() override {return false;}
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
};