#pragma once
#include "expression.hpp"

class Addition_Expression: public Binary_Operation_Expression
{
    const char* reg_id;
    public:
        Addition_Expression(): Binary_Operation_Expression("+", -6), reg_id("\\+"){}
        long evaluate(const DataContext* dc) const override;
        const char* get_reg_id() const override {return reg_id;}
        const char* to_cstr() override { return "Addition_Expression";}
};

class Substraction_Expression: public Binary_Operation_Expression
{
    public:
        Substraction_Expression(): Binary_Operation_Expression("-", -6){}
        long evaluate(const DataContext* dc) const override;
        const char* to_cstr() override { return "Substraction_Expression";}
};

class Multiplication_Expression: public Binary_Operation_Expression
{
    const char* reg_id;
    public:
        Multiplication_Expression(): Binary_Operation_Expression("*", -5), reg_id("\\*"){}
        long evaluate(const DataContext* dc) const override;
        const char* get_reg_id() const override {return reg_id;}
};

class Division_Expression: public Binary_Operation_Expression
{
    public:
        Division_Expression(): Binary_Operation_Expression("/", -5){}
        long evaluate(const DataContext* dc) const override;
};

class Modulo_Expression: public Binary_Operation_Expression
{
    public:
        Modulo_Expression(): Binary_Operation_Expression("%", -5){}
        long evaluate(const DataContext* dc) const override;
};