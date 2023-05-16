#pragma once

/**
 * @file automation.hpp
 * @author Mathias Tiberghien (mathias.tiberghien@thalesgroup.com)
 * @brief Defines generic classes used in automaton code generation
 * @version 0.1
 * @date 2023-05-14
 * 
 * @copyright © Thales Services Numériques 2023
 * 
 */

#include "container.hpp"
#include "data_context.hpp"
#include <string.h>
#include "expression.hpp"
#include "expr_mathematical.hpp"
#include "expr_logical.hpp"
#include "expr_functions.hpp"
#include "expr_affectation.hpp"

/**
 * @brief Defines an automaton device settings with settings that will be applied on setup and inputs/outputs definitions
 * 
 */
class DeviceSettings
{
    /**
     * @brief The device setup settings storing properties as a key value dictionary
     * 
     */
    Dictionary<const char*, long> config;
    /**
     * @brief The device inputs definitions mapping user defined keys with indexes
     * 
     */
    Dictionary<const char*, int> inputs;
    /**
     * @brief The device outputs definitions mapping user defined keys with indexes
     * 
     */
    Dictionary<const char*,int> outputs;
    public:
        DeviceSettings(){}
        /**
         * @brief Get the config object
         * 
         * @return a const reference to the device setup settings dictionnary
         */
        const Dictionary<const char*, long>& get_config() const {return config;}
        /**
         * @brief Get the inputs object
         * 
         * @return a const reference to the device inputs dictionnary
         */
        const Dictionary<const char*, int>& get_inputs() const {return inputs;}
        /**
         * @brief Get the outputs object
         * 
         * @return a const reference to the device outputs dictionnary
         */
        const Dictionary<const char*,int> get_outputs() const {return outputs;}
        /**
         * @brief Retrieve the information if the device should be considered as an inpout
         * 
         * @return true if inputs dictionary contains entries
         * @return false otherwise
         */
        bool is_input()const {return inputs.size()>0;}
        /**
         * @brief Retrieve the information if the device should be considered as an output
         * 
         * @return true if outputs dictionary contains entries
         * @return false otherwise
         */
        bool is_output()const {return outputs.size()>0;}
        /**
         * @brief Add an entry to the device setup settings
         * 
         * @param id The key of the setting
         * @param value The value of the setting
         */
        void add_config(const char* id, long value){config[id]=value;}
        /**
         * @brief Add an input definition to the device inputs definitions
         * 
         * @param id The key of the input. The value will be the index in the inputs definitions
         */
        void add_input(const char* id){inputs[id]=inputs.size();}
        /**
         * @brief Add an output definition to the device outputs definitions
         * 
         * @param id The key of the output. The value will be the index in the outputs definitions
         */
        void add_output(const char* id){outputs[id]=outputs.size();}
};

/**
 * @brief The device interface in an automaton program
 * 
 */
class IDevice
{
    protected:
        /**
         * @brief Identifier of the device
         * 
         */
        const char* id;
        /**
         * @brief Type name of the device
         * 
         */
        const char* type;
        /**
         * @brief The device settings
         * 
         */
        DeviceSettings settings;
    public:
        IDevice(const char* id, const char* type, const DeviceSettings& settings):id(id), type(type), settings(settings){}
        virtual ~IDevice(){}
        const char* get_id() const {return id;}
        const char* get_type() const {return type;}
        /**
         * @brief Checks if the provided settings contain the required information to use the device
         * 
         * @return true if the settings are valid
         * @return false otherwise
         */
        virtual bool are_settings_valid() const = 0;
        /**
         * @brief Initialises the device at setup time
         * 
         * @return true 
         * @return false 
         */
        virtual bool setup() = 0;
};

/**
 * @brief The input device interface in an automation program
 * 
 */
class IInput: public virtual IDevice
{
    public:
    IInput(){}
    /**
     * @brief Get the value of an input related to its definition key
     * 
     * @param value_id the user defined value identifier
     * @return long 
     */
    virtual int get_value(const char* value_id) const = 0;
};

/**
 * @brief The output device interface in an automation program
 * 
 */
class IOutput: public virtual IDevice
{
    public:
    IOutput(){}
    /**
     * @brief Set the value related to the value identifier
     * 
     * @param value_id The user define value identfier in outputs definitions
     * @param value The new desired value
     * @return true If the operation suceed
     * @return false otherwise
     */
    virtual bool set_value(const char* value_id, long value) = 0;
    /**
     * @brief Increase the value related to the value identifier by provided value
     * 
     * @param value_id The user define value identfier in outputs definitions
     * @param value The increase value
     * @return true If the operation suceed
     * @return false otherwise
     */
    virtual bool increase_value(const char* value_id, long value) =0;
    /**
     * @brief Decrease the value related to the value identifier by provided value
     * 
     * @param value_id The user define value identfier in outputs definitions
     * @param value The decrease value
     * @return true If the operation suceed
     * @return false otherwise
     */
    virtual bool decrease_value(const char* value_id, long value) =0;
};

/**
 * @brief Interface for device that are Inpout and Output in an automaton program
 * 
 */
class IInputOutput: public virtual IInput, public virtual IOutput
{
    public:
    IInputOutput(){}
};

/**
 * @brief The different device types in an automaton program
 * 
 */
enum Device_Type{INPUT_DEVICE, OUTPUT_DEVICE, INPUTOUTPUT_DEVICE, INVALID_DEVICE};
class IDeviceFactory
{
    public:
        IDeviceFactory(){}
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
                ref = strtok(NULL, " ");
                if(ref)
                {
                    strcpy(value_id, ref);
                }
            }
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
                case Device_Type::INPUT_DEVICE:
                    add_or_set_input(factory->buildInput(id, type, settings));
                    break;
                case Device_Type::OUTPUT_DEVICE:
                    add_or_set_output(factory->buildOutput(id, type, settings));
                    break;
                case Device_Type::INPUTOUTPUT_DEVICE:
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
        void clean()
        {
            if(if_expr)
            {
                delete if_expr;
                if_expr = nullptr;
            }
            if(then_expr)
            {
                delete then_expr;
                then_expr = nullptr;
            }
            if(else_expr)
            {
                delete else_expr;
                else_expr = nullptr;
            }
        }
    public:
        Behavior(Expression* if_expr, Expression* then_expr, Expression* else_expr = nullptr):
            if_expr(if_expr), then_expr(then_expr), else_expr(else_expr){}
        Behavior(const Behavior& other) = delete;
        Behavior(Behavior&& other) = delete;
        Behavior& operator=(const Behavior& other) = delete;
        Behavior& operator=(Behavior&& other) = delete;
        ~Behavior()
        {
            clean();
        }
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
