#include "automation.hpp"



class LED : public virtual IOutput
{
   
    private:
        long pin;
    public:
        LED(const char* id, const DeviceSettings& settings): IDevice(id, "LED", settings), pin(settings.get_config().at_or_default("pin", -1)){}

        bool are_settings_valid() const override
            {
                return settings.is_input() && settings.get_config().exists("pin");
            }
        
        bool setup() override
            {
                //pinMode(pin,OUTPUT_DEVICE);
                
                return true;
            }

         bool set_value(const char* value_id, long value) override
         {
              //digitalWrite(pin,value);
              return true;
         }

         bool increase_value(const char* value_id, long value) override
         {
            //digitalWrite(pin,value);
            return true;
         }
         bool decrease_value(const char* value_id, long value) override
         {
            //digitalWrite(pin,value);
            return true;
         }


};