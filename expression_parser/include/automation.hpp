#pragma once
#include "container.hpp"

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
        const Dictionary<const char*, long>& get_config(){return config;}
        const Dictionary<const char*, int>& get_inputs(){return inputs;}
        const Dictionary<const char*,int> get_outputs(){return outputs;}
};

class IDevice
{
    const char* id;
    const char* type;
    DeviceSettings settings;
    public:
        IDevice(const char* id, const char* type, const DeviceSettings& settings):id(id), type(type), settings(settings){}
        virtual ~IDevice(){}
        const char* get_id() const {return id;}
        const char* get_type() const {return type;}
        virtual bool are_settings_valid() const = 0;
};

class IInput: public virtual IDevice
{
    public:
    IInput(const char* id, const char* type, const DeviceSettings& settings):IDevice(id, type, settings){}
    virtual long get_value(const char* value_id) const = 0;
};

class IOutput: public virtual IDevice
{
    public:
    IOutput(const char* id, const char* type, const DeviceSettings& settings):IDevice(id, type, settings){}
    virtual bool set_value(unsigned short index, long value) = 0;
    virtual bool increase_value(unsigned short index, long value) =0;
    virtual bool decrease_value(unsigned short index, long value) =0;
};
