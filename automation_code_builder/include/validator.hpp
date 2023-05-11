#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>
#include <nlohmann/json-schema.hpp>

using json = nlohmann::json;
using json_validator = nlohmann::json_schema::json_validator;
using namespace std;

struct Validation_Json
{
    bool is_valid{true};
    string message{""};
    operator bool() const {return is_valid;}
};

json createSchema(string schema);
json_validator validSchema(string schema);
Validation_Json validJson(string schema, json file);

//chaine de responsabilité
class ValidationJsonHandler
{
    private:
        unique_ptr<ValidationJsonHandler> next{nullptr};
    protected:
        Validation_Json validate_next_item(const json& item) const noexcept
        {
            if(next)
            {
                return next->validate_item(item);
            }
            return Validation_Json();
        }

    public:
        virtual Validation_Json validate_item(const json& item) const noexcept = 0;
        void set_next_item(unique_ptr<ValidationJsonHandler>&& next_item){this->next = move(next);}
};

class TargetExists: public ValidationJsonHandler
{
    Validation_Json validate_item(const json& item) const noexcept override;
};

class DevicesExist: public ValidationJsonHandler
{
    Validation_Json validate_item(const json& item) const noexcept override;
};

class BehaviorsExist: public ValidationJsonHandler
{
    Validation_Json validate_item(const json& item) const noexcept override;
};
