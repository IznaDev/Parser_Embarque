#pragma once
#include <filesystem>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include "automation.hpp"

using namespace std;

class SupportedDevice
{
    string className;
    string includeFilename;
    Device_Type type;
    public:
    SupportedDevice(string className, string includeFilename, Device_Type type):
        className(className), includeFilename(includeFilename), type(type){}
    string get_className() const {return className;}
    string get_includeFileName() const {return includeFilename;}
    Device_Type get_type() const {return type;}
};

class DeviceCollection
{
    map<string, SupportedDevice> devices;
    public:
        DeviceCollection(){}
        DeviceCollection(const string& driver);
        bool is_supported(const string& device_typeName) const;
        const SupportedDevice& get_device(const string& device_typeName) const {return devices.at(device_typeName);}

};

class DriverCollection
{
    map<string, DeviceCollection> targets;
    public:
        DriverCollection();
        bool is_supported(const string& target) const;
        const DeviceCollection& get_target(const string& target) const {return targets.at(target);}
};

extern const DriverCollection supported_targets;