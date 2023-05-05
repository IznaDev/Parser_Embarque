#pragma once

#include "automation.hpp"
#include <random>
#include <iostream>
#include "code_builder.hpp"

using namespace std;

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
            cout << get_id() << " initialisé avec le settings 'pin' qui vaut " << settings.get_config().at_or_default("pin", -1) << endl;
            return true;
        }

        long get_value(const char* value_id) const override
        {
          return rand()%10;
        }
};

class TestOutputDevice: public virtual IOutput
{
    private:
        long value;
    public:
        TestOutputDevice(const char* id, const DeviceSettings& settings): IDevice(id, "TestOutput", settings){}
        bool are_settings_valid() const override
        {
            return settings.is_output() && settings.get_config().exists("pin");
        }

        bool init() override
        {
            cout << get_id() << " initialisé avec le settings 'pin' qui vaut " << settings.get_config().at_or_default("pin", -1) << endl;
            return true;
        }

        bool set_value(const char* value_i, long value) override
        {
            this->value = value;
            return true;
        }
        virtual bool increase_value(const char* value_i, long value) override
        {
            this->value+=value;
            return true;
        }
        virtual bool decrease_value(const char* value_i, long value) override
        {
            this->value-=value;
            return true;
        }
};

class TestInputOutputDevice: public virtual IInputOutput
{
    private:
        long value;
    public:
        TestInputOutputDevice(const char* id, const DeviceSettings& settings): IDevice(id, "TestInputOutput", settings){}
        bool are_settings_valid() const override
        {
            return settings.is_input() && settings.is_output() && settings.get_config().exists("pin") && settings.get_config().exists("val_init");
        }

        bool init() override
        {
            value = settings.get_config().at_or_default("val_init", 5);
            cout << get_id() << " initialisé avec:" << endl <<\
             "'pin' qui vaut " << settings.get_config().at_or_default("pin", -1) << endl <<\
             "'init_val' qui vaut " << settings.get_config().at_or_default("val_init", -1) << endl;
            return true;
        }

        bool set_value(const char* value_i, long value) override
        {
            this->value = value;
            return true;
        }
        virtual bool increase_value(const char* value_i, long value) override
        {
            this->value+=value;
            return true;
        }
        virtual bool decrease_value(const char* value_i, long value) override
        {
            this->value-=value;
            return true;
        }

        long get_value(const char* value_id) const override
        {
          return value;
        }
};

class TestDefaultFactory: public IDeviceFactory
{
    public:
        virtual IInput* buildInput(const char* id, const char* type, const DeviceSettings& settings) const override
        {
            return new TestInputDevice(id, settings);
        }
        virtual IOutput* buildOutput(const char* id, const char* type, const DeviceSettings& settings) const override
        {
            return new TestOutputDevice(id, settings);
        }
        virtual IInputOutput* buildInputOutput(const char* id, const char* type, const DeviceSettings& settings) const override
        {
            return new TestInputOutputDevice(id, settings);
        }
        Device_Type get_device_type(const char* type) const override
        {
            if(type == "testinput")
            {
                return Device_Type::INPUT;
            }
            else if(type == "testoutput")
            {
                return Device_Type::OUTPUT;
            }
            else if(type == "testinputoutput")
            {
                return Device_Type::INPUTOUTPUT;
            }
            else return Device_Type::INVALID;
        }
};

namespace Automation
{
    static IDeviceFactory* getFactory(const char* target)
    {
        return new TestDefaultFactory();
    }
}

class TestCodeBuilder
{
    public:
        TestCodeBuilder(){}
        void build(string json);
};