#include "parser.hpp"
#include "automation.hpp"
#include <iostream>

using namespace std;

int main()
{
    std::string json = "{...}";
    //1. Valider json avec schéma
    //2. Iterer dans l'objet json (ex: nlohmann)
    //3. Générer le code qui décrit les settings
    // exemple target {"target":"test", ....}
    //exemple device dans json {"id":"presence_1", "type":"HSCR501", "config":{"pin":5, "autre_param":3}, "inputs"["value"]}
    // doit donner

    IDeviceFactory* factory = Automation::getFactory("test");
    // A faire pour chaque device du json
    Dictionary<const char*, long> config;
    config["pin"] = 5;
    config["autre_param"] = 3;
    Dictionary<const char*, int> inputs;
    inputs["value"]=0;
    Dictionary<const char*, int> outputs;
    DeviceSettings s(config, inputs, outputs);
    DeviceDataContext dc;
    // En fonction des settings
    if(s.is_input() && ! s.is_output())
    {
        IInput* device = factory->buildInput("presence_1", "HSCR501", s);
        dc.add_or_set_input(device);
    }
    dc.init();

    MemoryDataContext mdc;
    mdc.add_or_set("presence_1.value", 18);
    

    Expression_Parser parser;
    auto result = parser.parse("${presence_1.value}");
    if(result)
    {
        for(int i=0;i<10;i++)
        {
            cout << "Device presence(dc): " << result.expression->evaluate(&dc) << endl;
            cout << "Device presence(mdc): " << result.expression->evaluate(&mdc) << endl;
        }
        
    }
    else
    {
        cout << "Il y a eu une erreur: " << result.error_message << endl;
    }
    return 0;
}