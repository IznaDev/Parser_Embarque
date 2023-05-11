#include "automation.hpp"


class HCSR501 : public virtual IInput
{
    public:
        HCSR501(const char* id, const DeviceSettings& settings): IDevice(id, "HC-SR501", settings), pin(settings.get_config().at_or_default("pin", -1)){}

        bool are_settings_valid() const override
            {
                return settings.is_input() && settings.get_config().exists("pin");
            }
        
        bool setup() override
            {
                pinMode(pin,INPUT_DEVICE);
                return true;
            }

        long get_value(const char* value_id) const override
            {
                long result = analogRead(pin);
                return result;
            }

    private:
        long pin;


};
