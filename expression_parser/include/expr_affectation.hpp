#pragma once
#include "expression.hpp"


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

class Set_Expression: public Affectation_Expression
{
    public:
        Set_Expression(): Affectation_Expression("="){}
        long evaluate(const DataContext* dc = nullptr) const override;
        bool update(DataContext* dc) override;
};

class Increase_Expression: public Affectation_Expression
{
    const char* reg_id;
    public:
        Increase_Expression(): Affectation_Expression("+="), reg_id("\\+="){}
        long evaluate(const DataContext* dc = nullptr) const override;
        bool update(DataContext* dc) override;
        const char* get_reg_id() const override {return reg_id;};
};

class Decrease_Expression: public Affectation_Expression
{
    const char* reg_id;
    public:
        Decrease_Expression(): Affectation_Expression("-="), reg_id("\\-="){}
        long evaluate(const DataContext* dc = nullptr) const override;
        bool update(DataContext* dc) override;
        const char* get_reg_id() const override {return reg_id;};
};