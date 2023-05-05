#include "parser.hpp"
#include "automation.hpp"
#include "automation_test.hpp"
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    cout << "argv[1]" << argv[1] << endl;
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
        DeviceSettings s;
        s.add_config("pin", 10);
        s.add_config("val_init",4);
        s.add_input("val");
        dc.add_or_set_device(factory, id, type, s);
    }
    dc.init();   

    Expression_Parser parser;
    auto result = parser.parse("${test_2.val}");
    auto result_if = parser.parse("${test_2.value} <= 0");
    auto result_then = parser.parse("${test_2.val} += 3");
    auto result_else = parser.parse("${test_2.val} -=2");
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