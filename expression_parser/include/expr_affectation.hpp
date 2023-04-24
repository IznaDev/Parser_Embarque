#include "expression.hpp"

template <typename To, typename From> 
std::unique_ptr<To> dynamic_unique_cast(std::unique_ptr<From>&& p) {
    if (To* cast = dynamic_cast<To*>(p.get()))
    {
        std::unique_ptr<To> result(cast);
        p.release();
        return result;
    }
    return std::unique_ptr<To>(nullptr); // or throw std::bad_cast() if you prefer
}

class Affectation_Expression: public Operation_Expression
{
    protected:
        unique_ptr<Reference_Expression> left_member{nullptr};
        unique_ptr<Expression> right_member{nullptr};
    public:
        Affectation_Expression(const string& id, int priority=-16): Operation_Expression(id, priority){}
        bool can_add_member() const override {return !(left_member && right_member);}
        bool has_left_member() const override {return true;}
        bool is_updatable() const override {return true;}
        void add_member(unique_ptr<Expression>&& expr) override {
            if(left_member)
            {
                right_member = move(expr);
            }
            else
            {
                
                left_member = dynamic_unique_cast<Reference_Expression>(move(expr));
            }
        }
        string to_string() const override {return id +"(" + left_member->to_string() + ", " + right_member->to_string() + ")";}
};

class Set_Expression: public Affectation_Expression
{
    public:
        Set_Expression(): Affectation_Expression("="){}
        long evaluate(const DataContext* dc = nullptr) const override;
        bool update(DataContext* dc) override;
        string to_string() const override;
};

class Increase_Expression: public Affectation_Expression
{
    public:
        Increase_Expression(): Affectation_Expression("+="){}
        long evaluate(const DataContext* dc = nullptr) const override;
        bool update(DataContext* dc) override;
        string to_string() const override;
        string get_reg_id() const override {return "\\"+id;};
};

class Decrease_Expression: public Affectation_Expression
{
    public:
        Decrease_Expression(): Affectation_Expression("-="){}
        long evaluate(const DataContext* dc = nullptr) const override;
        bool update(DataContext* dc) override;
        string to_string() const override;
        string get_reg_id() const override {return "\\"+id;};
};