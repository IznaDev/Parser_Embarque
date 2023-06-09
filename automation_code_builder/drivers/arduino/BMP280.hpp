#include "automation.hpp"

class BMP280 : public virtual IInput
{
    private:
        long pin;
    public:
        BMP280(const char* id, const DeviceSettings& settings)
            : IDevice(is, "BMP-280, settings"),
            pin(settings.get_config().at_or_default("pin", -1)){};
        
        bool are_settings_valid() const override
        {
            return settings.is_input() && settings.get_config().exists("pin");
        }

        bool setup() override
        {
            pinMode(pin, INPUT_DEVICE);
            return true;
        }

        long get_value(const char* value_id) const override
        {
            int index = settings.get_inputs().at_or_default(value_id, -1);
            long result = 0;
            if(index >=0)
            {
                switch (index)
                {
                    case 0:
                        //TODO lire pression (voir doc driver du capteur)
                        result = 1000; break;
                    case 1:
                        //TODO lire temperature
                        result = 21; break;
                    default:
                        result = 0; break;
                }
            }
            return result;
        }
}