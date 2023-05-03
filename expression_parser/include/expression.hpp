#pragma once

#include <string>
#include <sstream>
#include <memory>
#include "utils.hpp"
#include <cmath>
#include "data_context.hpp"
#include <numeric>
#include <vector>
#include <algorithm>

using namespace std;

class Expression
{
    public:
        virtual long evaluate(const DataContext* dc = nullptr) const = 0;
        virtual bool update(DataContext* dc=nullptr){return false;}
        virtual bool is_updatable() const {return false;}
        virtual string to_string() const = 0;
};

class Constant_Expression: public Expression
{
    private:
        string value;
    public:
        Constant_Expression(const string& value): value(value){}
        Constant_Expression(string&& value): value(move(value)){}
        long evaluate(const DataContext* dc) const override;
        string to_string() const override {return value;}
};

class Operation_Expression: public Expression
{
    protected:
        string id;
        int priority{0};
    public:
        Operation_Expression(const string& id, int priority=0): id(id), priority(priority){}
        bool operator <(const Operation_Expression& other){return this->priority < other.priority;}
        bool operator ==(const Operation_Expression& other){return this->priority == other.priority;}
        virtual void add_member(unique_ptr<Expression>&& expr) = 0;
        virtual bool can_add_member() const = 0;
        virtual bool has_left_member() const = 0;
        virtual string get_reg_id() const {return id;};
};


class Unary_Operation_Expression: public Operation_Expression
{
    protected:
    unique_ptr<Expression> right_member{nullptr};
    public:
        Unary_Operation_Expression(const string& id, int priority=0): Operation_Expression(id, priority){}
        bool can_add_member() const override {return !right_member;}
        bool has_left_member() const override {return false;}
        void add_member(unique_ptr<Expression>&& expr) override {
            right_member = move(expr);
        }
        string to_string() const override {return id +"(" + right_member->to_string() + ")";} 

};

class Binary_Operation_Expression: public Operation_Expression
{
    protected:
    unique_ptr<Expression> left_member{nullptr};
    unique_ptr<Expression> right_member{nullptr};
    public:
        Binary_Operation_Expression(const string& id, int priority=0): Operation_Expression(id, priority){}
        bool can_add_member() const override {return !(left_member && right_member);}
        bool has_left_member() const override {return true;}
        void add_member(unique_ptr<Expression>&& expr) override {
            if(left_member)
            {
                right_member = move(expr);
            }
            else
            {
                left_member = move(expr);
            }
        }
        string to_string() const override {return id +"(" + left_member->to_string() + ", " + right_member->to_string() + ")";} 

};

class Reference_Expression: public Expression
{
    private:
        string reference;
    public:
        Reference_Expression(const string& reference):reference(reference){};
        long evaluate(const DataContext* dc) const override;
        bool set(DataContext* dc, long value)
        {
            if(dc)
            {
                return dc->set(reference.c_str(), value);
            }
            return false;
        }
        bool increase(DataContext* dc, long value)
        {
            if(dc)
            {
                return dc->increase(reference.c_str(), value);
            }
            return false;
        }
        bool decrease(DataContext* dc, long value)
        {
            if(dc)
            {
                return dc->decrease(reference.c_str(), value);
            }
            return false;
        }
        string to_string() const override {return "ref(" + reference +")";}
};

