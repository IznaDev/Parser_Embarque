#include "automation.hpp"



class LED : public virtual IOutput
{
   
    private:
        long pin;
        int value{0};
    public:
        LED(const char* id, const DeviceSettings& settings): IDevice(id, "LED", settings), pin(settings.get_config().at_or_default("pin", -1)),
        value(settings.get_config().at_or_default("init_value", 0)){}

        bool are_settings_valid() const override
            {
                return settings.is_input() && settings.get_config().exists("pin");
            }
        
        bool setup() override
            {
                pinMode(pin,OUTPUT);
                digitalWrite(pin, value);
                return true;
            }

         bool set_value(const char* value_id, long value) override
         {
              this->value = value;
              digitalWrite(pin,value);
              return true;
         }

         bool increase_value(const char* value_id, long i_value) override
         {
            this->value+=i_value;
            digitalWrite(pin,value);
            return true;
         }
         bool decrease_value(const char* value_id, long d_value) override
         {
            this->value-=d_value;
            digitalWrite(pin,value);
            return true;
         }


};