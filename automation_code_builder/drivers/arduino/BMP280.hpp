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
            pinMode(pin, INPUT);
            return true;
        }

        long get_value(const char* value_id) const override
        {
            long result = analogRead(pin);
            return result;
        }
}