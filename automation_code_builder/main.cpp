#include "parser.hpp"
#include "arduino_context.hpp"
#include "automation_test.hpp"
#include "arduino_code_builder.hpp"


int main(int argc, char* argv[])
{
    cout << "argv[1]" << argv[1] << endl;
    if(argc < 2)
    {
        cout << "Please provide a .json file_path in argument" << endl;
        return 0;
    }
    string file_path(argv[1]);
    ifstream file(file_path);
    if(!file.good())
    {
        cout << "Provided file: '" << file_path << "'" << "doesn't exist" << endl;
        return 0;
    }
    json json = json::parse(file);
    ArduinoCodeBuilder builder;
    builder.build(json);
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
    
    TestFactory factory;
    // A faire pour chaque device du json

    const char* id = "sensor_1";
    const char* type = "testinput";
    Device_Type d_type = factory.get_device_type(type);
    DeviceDataContext dc;
    if(d_type!=Device_Type::INVALID)
    {
        DeviceSettings s;
        s.add_config("pin", 10);
        s.add_config("val_init",4);
        s.add_input("val");
        s.add_input("val2");
        s.add_input("intensite");
        dc.add_or_set_device(&factory, id, type, s);
    }
    dc.setup();
    Expression_Parser parser;
    auto result = parser.parse("${sensor_1.intensite}");
    auto result2 = parser.parse("${sensor_1.val2}");
    auto result3 = parser.parse("${sensor_1.toto}");
    if(result && result2 && result3)
    {
        for(int i=0;i<10;i++)
        {
            cout << "result: " << result.expression->evaluate(&dc) << endl;
            cout << "result2: " << result2.expression->evaluate(&dc) << endl;
            cout << "result3: " << result3.expression->evaluate(&dc) << endl;
        }
        
    }
    else
    {
        cout << "Il y a eu une erreur" << endl;
    }
    return 0;
}