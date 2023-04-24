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
};

class LessThanEqual_Expression: public Binary_Operation_Expression
{
    public:
        LessThanEqual_Expression(): Binary_Operation_Expression("<=", -9){}
        long evaluate(const DataContext* dc) const override;
};

class GreaterThan_Expression: public Binary_Operation_Expression
{
    public:
        GreaterThan_Expression(): Binary_Operation_Expression(">", -9){}
        long evaluate(const DataContext* dc) const override;
};

class GreaterThanEqual_Expression: public Binary_Operation_Expression
{
    public:
        GreaterThanEqual_Expression(): Binary_Operation_Expression(">=", -9){}
        long evaluate(const DataContext* dc) const override;
};

class And_Expression: public Binary_Operation_Expression
{
    public:
        And_Expression(): Binary_Operation_Expression("and", -14){}
        long evaluate(const DataContext* dc) const override;
};

class Or_Expression: public Binary_Operation_Expression
{
    public:
        Or_Expression(): Binary_Operation_Expression("or", -15){}
        long evaluate(const DataContext* dc) const override;
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
    public:
        XOR_Bitwise_Expression(): Binary_Operation_Expression("^", -12){}
        long evaluate(const DataContext* dc) const override;
        string get_reg_id() const override {return "\\"+id;}
};

class And_Bitwise_Expression: public Binary_Operation_Expression
{
    public:
        And_Bitwise_Expression(): Binary_Operation_Expression("&", -11){}
        long evaluate(const DataContext* dc) const override;
};

class Or_Bitwise_Expression: public Binary_Operation_Expression
{
    public:
        Or_Bitwise_Expression(): Binary_Operation_Expression("|", -13){}
        long evaluate(const DataContext* dc) const override;
        string get_reg_id() const override {return "\\"+id;}
};