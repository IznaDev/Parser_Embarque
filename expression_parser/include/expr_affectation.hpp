#pragma once
#include "expression.hpp"


class Set_Expression: public Affectation_Expression
{
    public:
        Set_Expression(): Affectation_Expression("="){}
        long evaluate(const DataContext* dc = nullptr) const override;
        bool update(DataContext* dc) override;
        const char* to_cstr() const override {return "Set_Expression";}
};

class Increase_Expression: public Affectation_Expression
{
    const char* reg_id;
    public:
        Increase_Expression(): Affectation_Expression("+="), reg_id("\\+="){}
        long evaluate(const DataContext* dc = nullptr) const override;
        bool update(DataContext* dc) override;
        const char* get_reg_id() const override {return reg_id;};
        const char* to_cstr() const override {return "Increase_Expression";}
};

class Decrease_Expression: public Affectation_Expression
{
    const char* reg_id;
    public:
        Decrease_Expression(): Affectation_Expression("-="), reg_id("\\-="){}
        long evaluate(const DataContext* dc = nullptr) const override;
        bool update(DataContext* dc) override;
        const char* get_reg_id() const override {return reg_id;};
        const char* to_cstr() const override {return "Decrease_Expression";}
};