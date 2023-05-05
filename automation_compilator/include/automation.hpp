#pragma once
#include "container.hpp"
#include "data_context.hpp"
#include <string.h>
#include "expression.hpp"
#include "expr_mathematical.hpp"
#include "expr_logical.hpp"
#include "expr_functions.hpp"
#include "expr_affectation.hpp"


class DeviceSettings
{
    Dictionary<const char*, long> config;
    Dictionary<const char*, int> inputs;
    Dictionary<const char*,int> outputs;
    public:
        DeviceSettings(){}
        const Dictionary<const char*, long>& get_config() const {return config;}
        const Dictionary<const char*, int>& get_inputs() const {return inputs;}
        const Dictionary<const char*,int> get_outputs() const {return outputs;}
        bool is_input()const {return inputs.size()>0;}
        bool is_output()const {return outputs.size()>0;}
        void add_config(const char* id, long value){config[id]=value;}
        void add_input(const char* id){inputs[id]=inputs.size();}
        void add_output(const char* id){outputs[id]=outputs.size();}
};

class IDevice
{
    protected:
        const char* id;
        const char* type;
        DeviceSettings settings;
    public:
        IDevice(const char* id, const char* type, const DeviceSettings& settings):id(id), type(type), settings(settings){}
        virtual ~IDevice(){}
        const char* get_id() const {return id;}
        const char* get_type() const {return type;}
        virtual bool are_settings_valid() const = 0;
        virtual bool setup() = 0;
};

class IInput: public virtual IDevice
{
    public:
    virtual long get_value(const char* value_id) const = 0;
};

class IOutput: public virtual IDevice
{
    public:
    virtual bool set_value(const char* value_i, long value) = 0;
    virtual bool increase_value(const char* value_i, long value) =0;
    virtual bool decrease_value(const char* value_i, long value) =0;
};

class IInputOutput: public virtual IInput, public virtual IOutput
{
};

enum Device_Type{INPUT, OUTPUT, INPUTOUTPUT, INVALID};
class IDeviceFactory
{
    public:
        virtual Device_Type get_device_type(const char* type) const = 0;
        virtual IInput* buildInput(const char* id, const char* type, const DeviceSettings& settings) const = 0;
        virtual IOutput* buildOutput(const char* id, const char* type, const DeviceSettings& settings) const  = 0;
        virtual IInputOutput* buildInputOutput(const char* id, const char* type, const DeviceSettings& settings) const = 0;
};

class DeviceDataContext : public DataContext
{
    private:

        Dictionary<const char*, IInput*> inputs;
        Dictionary<const char*, IOutput*> outputs;
        void split_reference(const char* reference, char* device_id, char* value_id) const
        {
            char* ref = new char[strlen(reference)+1];
            strcpy(ref, reference);
            ref = strtok(ref, ".");
            if(ref)
            {
                strcpy(device_id, ref);
                ref = strtok(ref, ".");
                if(ref)
                {
                    strcpy(value_id, ref);
                }
            }
            delete ref;
        }
    public:
        DeviceDataContext(){}
        ~DeviceDataContext()
        {
            for(int i=0;i<inputs.size();i++)
            {
                const char* key = inputs.get_key(i);
                //InputsOutputs are duplicate...
                if(outputs.exists(key))
                {
                    outputs.remove(key);
                }
                delete inputs[inputs.get_key(i)];
            }
            for(int i=0;i<outputs.size();i++)
            {
                delete outputs[outputs.get_key(i)];
            }
        }
        bool exists(const char* reference) const noexcept override 
        {
            char* device_id = new char[strlen(reference)];
            char* value_id = new char[strlen(reference)];
            split_reference(reference, device_id, value_id);
            bool result = inputs.exists(device_id) || outputs.exists(device_id);
            delete device_id;
            delete value_id;
            return result;
        }
        long evaluate(const char* reference) const noexcept override {
            char* device_id = new char[strlen(reference)];
            char* value_id = new char[strlen(reference)];
            split_reference(reference, device_id, value_id);
            long result = 0;
            IInput* input = inputs.at_or_default(device_id, nullptr);
            if(input)
            {
                result = input->get_value(value_id);
            }
            delete device_id;
            delete value_id;
            return result;
        }
        virtual bool set(const char* reference, long value) noexcept override 
        { 
            char* device_id = new char[strlen(reference)];
            char* value_id = new char[strlen(reference)];
            split_reference(reference, device_id, value_id);
            bool result = false;
            IOutput* output = outputs.at_or_default(device_id, nullptr);
            if(output)
            {
                result = output->set_value(value_id, value);
            }
            delete device_id;
            delete value_id;
            return result;
        }
        bool increase(const char* reference, long increase_value) noexcept override 
        { 
             char* device_id = new char[strlen(reference)];
            char* value_id = new char[strlen(reference)];
            split_reference(reference, device_id, value_id);
            bool result = false;
            IOutput* output = outputs.at_or_default(device_id, nullptr);
            if(output)
            {
                result = output->increase_value(value_id, increase_value);
            }
            delete device_id;
            delete value_id;
            return result;
        }
        bool decrease(const char* reference, long decrease_value) noexcept override 
        {
             char* device_id = new char[strlen(reference)];
            char* value_id = new char[strlen(reference)];
            split_reference(reference, device_id, value_id);
            bool result = false;
            IOutput* output = outputs.at_or_default(device_id, nullptr);
            if(output)
            {
                result = output->decrease_value(value_id, decrease_value);
            }
            delete device_id;
            delete value_id;
            return result;
        }
        int get_size() const noexcept override {return inputs.size(); + outputs.size();}
        const char* get_key(int index) const noexcept override
        {
            if(index<inputs.size())
            {
                return inputs.get_key(index);
            }
            else
            {
                return outputs.get_key(index - inputs.size());
            }
        }
        void add_or_set_input(IInput* input)
        {
            if(input)
            {
                IInput* i = inputs.at_or_default(input->get_id(), nullptr);
                if(i)
                {
                    delete i;
                    i = nullptr;
                }
                inputs[input->get_id()] = input;
            }
        }

        void add_or_set_output(IOutput* output)
        {
            if(output)
            {
                IOutput* o = outputs.at_or_default(output->get_id(), nullptr);
                if(o)
                {
                    delete o;
                    o = nullptr;
                }
                outputs[output->get_id()] = output;
            }
        }

        void add_or_set_inputoutput(IInputOutput* inputoutput)
        {
            add_or_set_input(inputoutput);
            add_or_set_output(inputoutput);
        }

        void add_or_set_device(const IDeviceFactory* factory, const char* id, const char* type, const DeviceSettings& settings )
        {
            Device_Type t= factory->get_device_type(type);
            // En fonction des settings
            switch(t)
            {
                case Device_Type::INPUT:
                    add_or_set_input(factory->buildInput(id, type, settings));
                    break;
                case Device_Type::OUTPUT:
                    add_or_set_output(factory->buildOutput(id, type, settings));
                    break;
                case Device_Type::INPUTOUTPUT:
                    add_or_set_inputoutput(factory->buildInputOutput(id, type, settings));    
                    break;
                default: break;
            }
        }
        bool setup()
        {
            bool result=true;
            for(int i=0;i<inputs.size();i++)
            {
                const char* key= inputs.get_key(i);
                if(!outputs.exists(key))
                {
                    result = inputs[key]->setup() && result;
                }
            }
            for(int i=0;i<outputs.size();i++)
            {
                const char* key= outputs.get_key(i);
                result = outputs[key]->setup() && result;
            }

            return result;
        }
        
};

class Behavior
{
    private:
        Expression* if_expr{nullptr};
        Expression* then_expr{nullptr};
        Expression* else_expr{nullptr};
    public:
        Behavior(Expression* if_expr, Expression* then_expr, Expression* else_expr = nullptr):
            if_expr(if_expr), then_expr(then_expr), else_expr(else_expr){}
        void process(DeviceDataContext* dc)
        {
            if(if_expr && if_expr->evaluate(dc))
            {
                if(then_expr)
                {
                    then_expr->update(dc);
                }
            }
            else
            {
                if(else_expr)
                {
                    else_expr->update(dc);
                };
            }
        }
};
