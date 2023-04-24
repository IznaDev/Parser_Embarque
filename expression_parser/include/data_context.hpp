#pragma once

/**
 * @file data_context.hpp
 * @author Mathias Tiberghien (mathias.tiberghien@thalesgroup.com)
 * @brief The datacontext is an interface that can retrieve or set variables references which need to be evaluated at runtime
 * @version 0.1
 * @date 2023-04-24
 * 
 * @copyright © Thales Services Numériques 2023
 * 
 */

#include <string>
#include <map>
#include <algorithm>
#include <vector>

using namespace std;

class DataContext
{
    public:
        virtual bool exists(const string& reference) const noexcept= 0;
        virtual long evaluate(const string& reference) const noexcept = 0;
        virtual bool set(const string& reference, long value) noexcept = 0;
        virtual bool increase(const string& reference, long increase_value) noexcept = 0;
        virtual bool decrease(const string& reference, long decrease_value) noexcept = 0;
        virtual vector<string> get_keys() const noexcept = 0;
};

class MemoryDataContext: public DataContext
{
    private:
        map<string, long> values;
    public:
        bool exists(const string& reference) const noexcept override { return values.find(reference)!= values.end();}
        long evaluate(const string& reference) const noexcept override {return values.at(reference);}
        virtual bool set(const string& reference, long value) noexcept override 
        { 
            if(exists(reference))
            {
                values[reference]=value;
                return true;
            }
            return false;}
        virtual bool increase(const string& reference, long increase_value) noexcept override { if(exists(reference)){values[reference]+=increase_value; return true;} return false;}
        virtual bool decrease(const string& reference, long increase_value) noexcept override { if(exists(reference)){values[reference]-=increase_value; return true;} return false;}
        vector<string> get_keys() const noexcept override{
            vector<string> result;
            for(const auto& p: values)
            {
                result.push_back(p.first);
            }
            return result;
        }
        void add_or_set(const string& reference, long value)
        {
            values[reference] = value;
        }
        operator map<string,long>() const {return values;}
};