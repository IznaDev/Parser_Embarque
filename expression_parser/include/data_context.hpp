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

#include "container.hpp"
#include <string.h>

using namespace std;

class DataContext
{
    public:
        virtual bool exists(const char* reference) const noexcept= 0;
        virtual long evaluate(const char* reference) const noexcept = 0;
        virtual bool set(const char* reference, long value) noexcept = 0;
        virtual bool increase(const char* reference, long increase_value) noexcept = 0;
        virtual bool decrease(const char* reference, long decrease_value) noexcept = 0;
        virtual int get_size() const noexcept = 0;
        virtual const char* get_key(int index) const noexcept = 0;
};

class MemoryDataContext: public DataContext
{
    private:
        Dictionary<const char*, long> values;
    public:
        bool exists(const char* reference) const noexcept override {return values.exists(reference);}
        long evaluate(const char* reference) const noexcept override {
            return values.at_or_default(reference, 0);
        }
        virtual bool set(const char* reference, long value) noexcept override 
        { 
            return values.set(reference, value);
        }
        bool increase(const char* reference, long increase_value) noexcept override 
        { 
            if(values.exists(reference))
            {
                values[reference]+=increase_value;
                return true;
            }
            return false;
        }
        bool decrease(const char* reference, long decrease_value) noexcept override 
        {
            if(values.exists(reference))
            {
                values[reference]-=decrease_value;
                return true;
            }
            return false;
        }
        int get_size() const noexcept override {return values.size();}
        const char* get_key(int index) const noexcept override
        {
            return values.get_key(index);
        }
        void add_or_set(const char* reference, long value)
        {
            values[reference] = value;
        }
};