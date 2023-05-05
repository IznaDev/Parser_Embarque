#pragma once
#include "expression.hpp"

class Equals_Expression: public Binary_Operation_Expression
{
    public:
        Equals_Expression(): Binary_Operation_Expression("==", -10){}
        long evaluate(const DataContext* dc) const override;
};

class NotEquals_Expression: public Binary_Operation_Expression
{
    public:
        NotEquals_Expression(): Binary_Operation_Expression("!=", -10){}
        long evaluate(const DataContext* dc) const override;
};

class LessThan_Expression: public Binary_Operation_Expression
{
    public:
        LessThan_Expression(): Binary_Operation_Expression("<", -9){}
        long evaluate(const DataContext* dc) const override;
        const char* to_cstr() override {return "LessThan_Expression";}
};

class LessThanEqual_Expression: public Binary_Operation_Expression
{
    public:
        LessThanEqual_Expression(): Binary_Operation_Expression("<=", -9){}
        long evaluate(const DataContext* dc) const override;
        const char* to_cstr() override {return "LessThanEqual_Expression";}
};

class GreaterThan_Expression: public Binary_Operation_Expression
{
    public:
        GreaterThan_Expression(): Binary_Operation_Expression(">", -9){}
        long evaluate(const DataContext* dc) const override;
        const char* to_cstr() override {return "GreaterThan_Expression";}
};

class GreaterThanEqual_Expression: public Binary_Operation_Expression
{
    public:
        GreaterThanEqual_Expression(): Binary_Operation_Expression(">=", -9){}
        long evaluate(const DataContext* dc) const override;
        const char* to_cstr() override {return "GreaterThanEqual_Expression";}
};

class And_Expression: public Binary_Operation_Expression
{
    public:
        And_Expression(): Binary_Operation_Expression("and", -14){}
        long evaluate(const DataContext* dc) const override;
        const char* to_cstr() override {return "And_Expression";}
};

class Or_Expression: public Binary_Operation_Expression
{
    public:
        Or_Expression(): Binary_Operation_Expression("or", -15){}
        long evaluate(const DataContext* dc) const override;
        const char* to_cstr() override {return "Or_Expression";}
};

class Not_Expression: public Unary_Operation_Expression
{
    public:
        Not_Expression(): Unary_Operation_Expression("!", 3){}
        long evaluate(const DataContext* dc) const override;
};

class Shift_Left_Expression: public Binary_Operation_Expression
{
    public:
        Shift_Left_Expression(): Binary_Operation_Expression("<<", -7){}
        long evaluate(const DataContext* dc) const override;
};

class Shift_Right_Expression: public Binary_Operation_Expression
{
    public:
        Shift_Right_Expression(): Binary_Operation_Expression(">>", -7){}
        long evaluate(const DataContext* dc) const override;
};

class XOR_Bitwise_Expression: public Binary_Operation_Expression
{
    const char* reg_id;
    public:
        XOR_Bitwise_Expression(): Binary_Operation_Expression("^", -12), reg_id("\\^"){}
        long evaluate(const DataContext* dc) const override;
        const char* get_reg_id() const override {return reg_id;}
};

class And_Bitwise_Expression: public Binary_Operation_Expression
{
    public:
        And_Bitwise_Expression(): Binary_Operation_Expression("&", -11){}
        long evaluate(const DataContext* dc) const override;
};

class Or_Bitwise_Expression: public Binary_Operation_Expression
{
    const char* reg_id;
    public:
        Or_Bitwise_Expression(): Binary_Operation_Expression("|", -13), reg_id("\\|"){}
        long evaluate(const DataContext* dc) const override;
        const char* get_reg_id() const override {return reg_id;}
};