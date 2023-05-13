#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>
#include <nlohmann/json-schema.hpp>
#include "parser.hpp"
#include <memory>
#include "driver.hpp"
#include <string>
#include <set>
#include "parser.hpp"

using namespace std;
using json = nlohmann::json;
using json_validator = nlohmann::json_schema::json_validator;

struct DeviceRef
{
    set<string> inputs;
    set<string> outputs;
};


class JSONValidationHandler
{
    private:
        unique_ptr<JSONValidationHandler> next{nullptr};
    protected:
        Validation_Result validate_next(const json& json) const noexcept
        {       
            if(next)
            {
                return next->validate(json);
            }
            return Validation_Result();
        }

    public:
        virtual Validation_Result validate(const json& json) const noexcept = 0;
        void set_next(unique_ptr<JSONValidationHandler>&& next){this->next = move(next);}
};

class Schema_JSONValidationHandler: public JSONValidationHandler
{
    string schema_file_path;
    json_validator validator;
    json createSchema() const;
    public:
        Schema_JSONValidationHandler(const string& schema_file_path): schema_file_path(schema_file_path)
        {
            validator.set_root_schema(createSchema());
        }
        Validation_Result validate(const json& file) const noexcept override;
};

class Target_JSONValidationHandler: public JSONValidationHandler
{
    public:
        Validation_Result validate(const json& file) const noexcept override;
};

class Device_JSONValidationHandler: public JSONValidationHandler
{
    Validation_Result validate(const json& file) const noexcept override;
};

class Expression_JSONValidationHandler: public JSONValidationHandler
{
   Validation_Result validate(const json& file) const noexcept override; 
};

class Automation_Validator
{
    unique_ptr<JSONValidationHandler> validation_handler{nullptr};
    public:
        Automation_Validator(const string& schema_file_path);
        Validation_Result validate(const json& file) const noexcept;
};

class ValidatorExpressionVisitor: public IExpressionVisitor
{
    json file;
    map<string, DeviceRef> device_refs;
    Validation_Result v_result;
    bool look_for_outputs{false};
    public:
        ValidatorExpressionVisitor(const json& file): file(file)
        {
            for(const auto& d: file["devices"])
            {
                string id = d["id"];
                DeviceRef& d_ref = device_refs[id];
                if(d.find("inputs")!= d.end())
                {
                    for(const string& i: d["inputs"])
                    {
                        d_ref.inputs.emplace(i);
                    }
                }
                if(d.find("outputs")!= d.end())
                {
                    for(const string& i: d["outputs"])
                    {
                        d_ref.outputs.emplace(i);
                    }
                }
            } 
        }
        void visit(const Expression* expr) override;
        void visit(const Constant_Expression* expr) override;
        void visit(const Reference_Expression* expr) override;
        void visit(const Binary_Operation_Expression* expr) override;
        void visit(const Affectation_Expression* expr) override;
        void visit(const Unary_Operation_Expression* expr) override;
        void visit(const Function_Expression* expr) override;
        const Validation_Result& get_v_result() const {return v_result;}
};
