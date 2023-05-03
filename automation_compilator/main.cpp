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
    //exemple device dans json:
    /* {
            "id":"test_2",
            "type":"testinputoutput",
            "config":
            {
                "pin":10,
                "val_init":3
            },
            "inputs":["val"],
            "outputs":["val"]
    } */
    // doit donner

    IDeviceFactory* factory = Automation::getFactory("test");
    // A faire pour chaque device du json

    const char* id = "test_2";
    const char* type = "testinputoutput";
    Device_Type d_type = factory->get_device_type(type);
    DeviceDataContext dc;
    if(d_type!=Device_Type::INVALID)
    {
        Dictionary<const char*, long> config;
        config["pin"] = 10;
        config["val_init"] = 3;
        Dictionary<const char*, int> inputs;
        //la valeur à renseigner est l'index de val dans le tableau inputs
        inputs["val"]=0;
        Dictionary<const char*, int> outputs;
        //la valeur à renseigner est l'index de val dans le tableau outputs
        outputs["val"]=0;
        DeviceSettings s(config, inputs, outputs);

        dc.add_or_set_device(factory, id, type, s);
    }
    dc.init();   

    Expression_Parser parser;
    auto result = parser.parse("${test_2.val}");
    auto result_if = parser.parse("${test_2.value} <= 0");
    auto result_then = parser.parse("${test_2.val} += 2");
    auto result_else = parser.parse("${test_2.val} -=5");
    if(result && result_if && result_then && result_else)
    {
        for(int i=0;i<10;i++)
        {
            cout << "Device value: " << result.expression->evaluate(&dc) << endl;
            long if_val = result_if.expression->evaluate(&dc);
            cout << "If ${test_2.value} <= 0: " << if_val << endl;
            if(if_val)
            {
                result_then.expression->update(&dc);
            }
            else
            {
                result_else.expression->update(&dc);
            }
        }
        
    }
    else
    {
        cout << "Il y a eu une erreur" << endl;
    }
    return 0;
}