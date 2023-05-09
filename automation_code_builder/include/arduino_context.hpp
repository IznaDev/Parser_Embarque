#include "automation.hpp"
#include "hcsr501.hpp"
#include "LED.hpp"



class ArduinoFactory: public IDeviceFactory
{
    
      
    public:
          ArduinoFactory(){};

          virtual IInput* buildInput(const char* id, const char* type, const DeviceSettings& settings) const override
          {
            if(type == "HC-SR501")
              return new HCSR501(id, settings);

            return nullptr;
          }
      
          virtual IOutput* buildOutput(const char* id, const char* type, const DeviceSettings& settings) const override
          {
            if (type == "LED")
              return new LED(id, settings);

            return nullptr;
          }

          virtual IInputOutput* buildInputOutput(const char* id, const char* type, const DeviceSettings& settings) const override
          {
            return nullptr;
          }

        Device_Type get_device_type(const char* type) const override
        {
            if(type == "HC-SR501")
            {
                return Device_Type::INPUT;
            }
            else if(type == "LED")
            {
                return Device_Type::OUTPUT;
            }
            
            else return Device_Type::INVALID;
        }
};