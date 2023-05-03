#pragma once
#include "container.hpp"
#include "data_context.hpp"
#include <string.h>



class DeviceSettings
{
    Dictionary<const char*, long> config;
    Dictionary<const char*, int> inputs;
    Dictionary<const char*,int> outputs;
    public:
        DeviceSettings(const Dictionary<const char*, long>& config, const Dictionary<const char*, int>& inputs, const Dictionary<const char*,int>& outputs):
        config(config), inputs(inputs), outputs(outputs){}
        DeviceSettings(Dictionary<const char*, long>&& config, Dictionary<const char*, int>&& inputs, Dictionary<const char*,int>&& outputs):
        config(config), inputs(inputs), outputs(outputs){}
        const Dictionary<const char*, long>& get_config() const {return config;}
        const Dictionary<const char*, int>& get_inputs() const {return inputs;}
        const Dictionary<const char*,int> get_outputs() const {return outputs;}
        bool is_input()const {return inputs.size()>0;}
        bool is_output()const {return outputs.size()>0;}
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
        virtual bool init() = 0;
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
    public:
    IInputOutput(const char* id, const char* type, const DeviceSettings& settings):IDevice(id, type, settings){}
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
        bool init()
        {
            for(int i=0;i<inputs.size();i++)
            {
                const char* key= inputs.get_key(i);
                inputs[key]->init();
            }
            for(int i=0;i<outputs.size();i++)
            {
                const char* key= outputs.get_key(i);
                outputs[key]->init();
            }
        }
        
};

class IDeviceFactory
{
    public:
        virtual IInput* buildInput(const char* id, const char* type, const DeviceSettings& settings) = 0;
        virtual IOutput* buildOutput(const char* id, const char* type, const DeviceSettings& settings) = 0;
        virtual IInputOutput* buildInputOutput(const char* id, const char* type, const DeviceSettings& settings) = 0;
};
#include <random>

class TestInputDevice: public virtual IInput
{
    public:
        TestInputDevice(const char* id, const DeviceSettings& settings): IDevice(id, "TestInput", settings){}
        bool are_settings_valid() const override
        {
            return settings.is_input() && settings.get_config().exists("pin");
        }

        bool init() override
        {
            cout << "J'ai initialisé avec le settings 'pin' qui vaut " << settings.get_config().at_or_default("pin", -1) << endl;
            return true;
        }

        long get_value(const char* value_id) const override
        {
          return rand()%10;
        }
};

class TestDefaultFactory: public IDeviceFactory
{
    public:
        virtual IInput* buildInput(const char* id, const char* type, const DeviceSettings& settings) override
        {
            return new TestInputDevice(id, settings);
        }
        virtual IOutput* buildOutput(const char* id, const char* type, const DeviceSettings& settings)
        {
            throw logic_error("Not implemented");
        }
        virtual IInputOutput* buildInputOutput(const char* id, const char* type, const DeviceSettings& settings)
        {
            throw logic_error("Not implemented");
        }
};

namespace Automation
{
    static IDeviceFactory* getFactory(const char* target)
    {
        return new TestDefaultFactory();
    }
}
