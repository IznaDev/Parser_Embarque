#pragma once

#include <cstdlib>
#include "data_context.hpp"

bool is_whitespace_or_empty(const char* text);

class Expression
{
    public:
        virtual long evaluate(const DataContext* dc = nullptr) const = 0;
        virtual bool update(DataContext* dc=nullptr){return false;}
        virtual bool is_updatable() const {return false;}
};

class Constant_Expression: public Expression
{
    private:
        long value;
    public:
        Constant_Expression(const char* value): value(is_whitespace_or_empty(value) ? 0: atol(value)){}
        long evaluate(const DataContext* dc) const override;
};

class Reference_Expression: public Expression
{
    private:
        char* reference;
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
};

class Binary_Operation_Expression: public Operation_Expression
{
    protected:
    Expression* left_member{nullptr};
    Expression* right_member{nullptr};
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
};