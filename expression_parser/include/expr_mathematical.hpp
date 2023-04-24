#pragma once

#include "expression.hpp"

class Addition_Expression: public Binary_Operation_Expression
{
    public:
        Addition_Expression(): Binary_Operation_Expression("+", -6){}
        long evaluate(const DataContext* dc) const override;
        string get_reg_id() const override {return "\\"+id;}
};

class Substraction_Expression: public Binary_Operation_Expression
{
    public:
        Substraction_Expression(): Binary_Operation_Expression("-", -6){}
        long evaluate(const DataContext* dc) const override;
};

class Multiplication_Expression: public Binary_Operation_Expression
{
    public:
        Multiplication_Expression(): Binary_Operation_Expression("*", -5){}
        long evaluate(const DataContext* dc) const override;
        string get_reg_id() const override {return "\\"+id;}
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