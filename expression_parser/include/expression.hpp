#pragma once

#include <stdlib.h>
#include "data_context.hpp"

bool is_whitespace_or_empty(const char* text);


class IExpressionVisitor;


class Expression
{
    protected:
        virtual void clean(){}
    public:
        virtual ~Expression(){ clean();}
        virtual long evaluate(const DataContext* dc = nullptr) const = 0;
        virtual bool update(DataContext* dc=nullptr){return false;}
        virtual bool is_updatable() const {return false;}
        virtual Expression* simplify() = 0;
        virtual bool has_variable() const  = 0;
        virtual bool is_leaf() const = 0;
        virtual const char* to_cstr() const{ return "Expression";}
        virtual void accept(IExpressionVisitor* visitor) const {}
};

class Constant_Expression: public Expression
{
    private:
        long value;
    public:
        Constant_Expression(const char* value): value(is_whitespace_or_empty(value) ? 0: atol(value)){}
        Constant_Expression(long value): value(value){}
        long evaluate(const DataContext* dc) const override;
        bool has_variable() const  override{return false;}
        bool is_leaf() const override{return true;}
        Expression* simplify(){return this;}
        const char* to_cstr() const override { return "Constant_Expression";}
        void accept(IExpressionVisitor* visitor) const override;
        long get_value() const {return value;}
};

class Reference_Expression: public Expression
{
    private:
        char* reference;
    protected:
        virtual void clean() override { delete[] reference;}
    public:
        Reference_Expression(const char* ref):reference(new char[strlen(ref)+1]){
            strcpy(reference, ref);
        };
        long evaluate(const DataContext* dc) const override;
        bool set(DataContext* dc, long value)
        {
            if(dc)
            {
                return dc->set(reference, value);
            }
            return false;
        }
        bool increase(DataContext* dc, long value)
        {
            if(dc)
            {
                return dc->increase(reference, value);
            }
            return false;
        }
        bool decrease(DataContext* dc, long value)
        {
            if(dc)
            {
                return dc->decrease(reference, value);
            }
            return false;
        }
        bool has_variable() const override{return true;}
        bool is_leaf() const override{return true;}
        Expression* simplify(){return this;}
        const char* to_cstr() const override { return "Reference_Expression";}
        void accept(IExpressionVisitor* visitor) const override;
        const char* get_ref() const {return reference;}
};

class Operation_Expression: public Expression
{
    protected:
        const char* id;
        int priority{0};
    public:
        Operation_Expression(const char* id, int priority=0): id(id), priority(priority){}
        bool operator <(const Operation_Expression& other){return this->priority < other.priority;}
        bool operator ==(const Operation_Expression& other){return this->priority == other.priority;}
        virtual void add_member(Expression* expr) = 0;
        virtual void add_ref_member(Reference_Expression* ref_expr) = 0;
        virtual bool can_add_member() const = 0;
        virtual bool has_left_member() const = 0;
        virtual const char* get_reg_id() const {return id;};
};

class Unary_Operation_Expression: public Operation_Expression
{
    protected:
    Expression* right_member{nullptr};
    void clean() override { if(right_member) {delete right_member; right_member= nullptr;}}
    public:
        Unary_Operation_Expression(const char* id, int priority=0): Operation_Expression(id, priority){}
        bool can_add_member() const override {return !right_member;}
        bool has_left_member() const override {return false;}
        void add_member(Expression* expr) override {
            if(!right_member)
            {
                right_member = expr;
            }
        }
        void add_ref_member(Reference_Expression* expr) override {
            if(!right_member)
            {
                right_member = expr;
            }
        }
        Expression* simplify() override
        {
            if(right_member)
            {
                if(!right_member->is_leaf())
                {
                    Expression* expr = right_member->simplify();
                    if(expr!=right_member)
                    {
                        delete right_member;
                        right_member = expr;
                    }
                }
                if(!right_member->has_variable())
                {
                    return new Constant_Expression(this->evaluate());
                }
            }
            return this;
        }
        bool has_variable() const override{return !right_member || right_member->has_variable();}
        bool is_leaf() const override{return false;}
        void accept(IExpressionVisitor* visitor) const override;
        const Expression* get_right_member() const {return right_member;}
};

class Binary_Operation_Expression: public Operation_Expression
{
    protected:
    Expression* left_member{nullptr};
    Expression* right_member{nullptr};
    void clean() override
    {
        if(left_member)
        {
            delete left_member;
            left_member = nullptr;
        }
        if(right_member)
        {
            delete right_member;
            right_member = nullptr;
        }
    }
    public:
        Binary_Operation_Expression(const char* id, int priority=0): Operation_Expression(id, priority){}
        bool can_add_member() const override {return !(left_member && right_member);}
        bool has_left_member() const override {return true;}
        void add_member(Expression* expr) override {
            if(left_member)
            {
                right_member = expr;
            }
            else
            {
                left_member = expr;
            }
        }
        void add_ref_member(Reference_Expression* ref_expr) override
        {
            if(left_member)
            {
                right_member = ref_expr;
            }
            else
            {
                left_member = ref_expr;
            }
        }
        Expression* simplify() override
        {
            if(left_member)
            {
                if(!left_member->is_leaf())
                {
                    Expression* expr = left_member->simplify();
                    if(expr!=left_member)
                    {
                        delete left_member;
                        left_member = expr;
                    }
                }
            }
            if(right_member)
            {
                if(!right_member->is_leaf())
                {
                    Expression* expr = right_member->simplify();
                    if(expr!=right_member)
                    {
                        delete right_member;
                        right_member = expr;
                    }
                }
            }
            if(!has_variable())
            {
                return new Constant_Expression(this->evaluate()); 
            }

            return this;
        }
        bool has_variable() const override {return (!left_member || left_member->has_variable()) || (!right_member || right_member->has_variable()); }
        bool is_leaf() const override{return false;}
        void accept(IExpressionVisitor* visitor) const override;
        const Expression* get_left_member() const {return left_member;}
        const Expression* get_right_member() const {return right_member;}
};

class Function_Expression: public Expression
{
    protected:
        static const int max_size=256;
        const char* id;
        int last_index{0};
        Expression** args;
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
            delete[] args;
            last_index = 0;
        }
    public:
        Function_Expression(const char* id): id(id){};
        void add_arg(Expression* expr){
            if(last_index<max_size)
            {
                Expression** new_args=new Expression*[last_index+1];
                for(int i=0;i<last_index;i++)
                {
                    new_args[i]=args[i];
                }
                args = new_args;
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
        virtual bool has_variable() const override
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
        virtual bool is_leaf() const override {return false;}
        void accept(IExpressionVisitor* visitor) const override;
        int args_size() const {return last_index;}
        const Expression* get_arg(int index) const
        {
            return args[index];
        }
};

class Affectation_Expression: public Operation_Expression
{
    protected:
        Reference_Expression* left_member{nullptr};
        Expression* right_member{nullptr};
        void clean() override 
        {
            if(left_member)
            {
                delete left_member;
                left_member = nullptr;
            }
            if(right_member)
            {
                delete right_member;
                right_member = nullptr;
            }
        }
    public:
        Affectation_Expression(const char* id, int priority=-16): Operation_Expression(id, priority){}
        bool can_add_member() const override {return !(left_member && right_member);}
        bool has_left_member() const override {return true;}
        bool is_updatable() const override {return true;}
        void add_member(Expression* expr) override {
            if(left_member)
            {
                right_member = expr;
            }
        }
        void add_ref_member(Reference_Expression* ref_expr) override
        {
            if(left_member)
            {
                right_member = ref_expr;
            }
            else
            {
                
                left_member = ref_expr;
            }
        }
        bool has_variable() const override {return true;}
        Expression* simplify() override
        {
            if(right_member)
            {
                if(!right_member->is_leaf())
                {
                    Expression* expr = right_member->simplify();
                    if(expr!=right_member)
                    {
                        delete right_member;
                        right_member = expr;
                    }
                }
            }
            if(!has_variable())
            {
                return new Constant_Expression(this->evaluate()); 
            }

            return this;
        }
        bool is_leaf() const override {return false;}
        void accept(IExpressionVisitor* visitor) const override;
        const Reference_Expression* get_left_member() const {return left_member;}
        const Expression* get_right_member() const {return right_member;}
};

class Custom_Function_Expression: public Function_Expression
{
    private: 
        Expression* expression;
        static const char* var_args[Function_Expression::max_size];
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
        const Expression* get_expression() const {return expression;}
};

class IExpressionVisitor
{
    public:
        virtual void visit(const Expression* expr) = 0;
        virtual void visit(const Constant_Expression* expr) = 0;
        virtual void visit(const Reference_Expression* expr) = 0;
        virtual void visit(const Binary_Operation_Expression* expr) = 0;
        virtual void visit(const Affectation_Expression* expr) = 0;
        virtual void visit(const Unary_Operation_Expression* expr) = 0;
        virtual void visit(const Function_Expression* expr) = 0;
};