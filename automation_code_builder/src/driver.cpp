#include "driver.hpp"

const filesystem::path driver_directory{"../../automation_code_builder/drivers"};
const DriverCollection supported_targets;

DeviceCollection::DeviceCollection(const string& driver)
{
    filesystem::path device_directory = driver_directory / filesystem::path(driver);
    ifstream supported_drivers{device_directory/filesystem::path{"supported_drivers.txt"}};
    if(supported_drivers.good())
    {
        string line;
        //skip headers
        getline(supported_drivers, line);
        while(getline(supported_drivers, line))
        {
            istringstream l{line};
            string device_typeName;
            string className;
            string include_fileName;
            int type;
            l >> device_typeName >> className >> include_fileName >> type;
            devices.insert({device_typeName, SupportedDevice(className, include_fileName, static_cast<Device_Type>(type))});
        }
    }
}

bool DeviceCollection::is_supported(const string& device_typeName) const
{
    return devices.find(device_typeName) != devices.end();
}

DriverCollection::DriverCollection()
{
    for (auto const& dir_entry : std::filesystem::directory_iterator{driver_directory}) 
    {
        if(dir_entry.is_directory())
        {
            string driver = dir_entry.path().stem().string();
            targets[driver]=DeviceCollection(driver);
        }
    } 
}

bool DriverCollection::is_supported(const string& driver) const
{
    return targets.find(driver) != targets.end();
}