#include "expression.hpp"

class Function_Expression: public Expression
{
    protected:
        string id;
        vector<unique_ptr<Expression>> args;
    public:
        Function_Expression(const string& id): id(id){};
        void add_arg(unique_ptr<Expression>&& expr){
            args.push_back(move(expr));
        }

        string to_string() const override 
        { 
            ostringstream s;
            s << id << "(";
            for(int i=0;i<args.size();i++)
            {
                s << args[i]->to_string();
                if(i<args.size()-1)
                {
                    s << ",";
                }
            }
            s << ")";
            return s.str();
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
        unique_ptr<Expression> expression;
    public:
        Custom_Function_Expression(const string& id, unique_ptr<Expression>&& func): Function_Expression(id), expression(move(func)){}
        long evaluate(const DataContext* dc) const override;
};