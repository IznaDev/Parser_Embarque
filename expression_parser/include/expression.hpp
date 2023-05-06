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
        virtual bool has_variable() = 0;
        virtual bool is_leaf() = 0;
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
        bool has_variable() override{return false;}
        bool is_leaf() override{return true;}
        Expression* simplify(){return this;}
        const char* to_cstr() const override { return "Constant_Expression";}
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
        bool has_variable() override{return true;}
        bool is_leaf() override{return true;}
        Expression* simplify(){return this;}
        const char* to_cstr() const override { return "Reference_Expression";}
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
        bool has_variable() override{return !right_member || right_member->has_variable();}
        bool is_leaf() override{return false;}
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
        bool has_variable() override {return (!left_member || left_member->has_variable()) || (!right_member || right_member->has_variable()); }
        bool is_leaf() override{return false;}
        void accept(IExpressionVisitor* visitor) const override;
};

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
        bool has_variable() override {return true;}
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
        bool is_leaf() override {return false;}
};

class IExpressionVisitor
{
    public:
        virtual void visit(const Expression* expr) = 0;
        virtual void visit(const Binary_Operation_Expression* expr) = 0;
};