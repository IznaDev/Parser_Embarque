#include "arduino_code_builder.hpp"
#include "parser.hpp"

#include <iostream>
using namespace std;

void add_global_declarations(ostream& output, const json& data)
{
    output << endl;
    output << "// Global variables" << endl;
    output << "ArduinoFactory factory;" << endl;
    output << "DeviceDataContext dc;" << endl;
    json b = data["behaviors"];
    int nb_behaviors = b.size();
    output << "const int nb_behaviors = " << nb_behaviors << ";" << endl;
    output << "Behavior* behaviors[nb_behaviors];" << endl;
    output << endl;
}

string get_factory_include()
{
    return "arduino_context.hpp";
}

void add_config(ofstream& output, const string& s_id, const json& config)
{
    for(json::const_iterator it = config.begin(); it!=config.end(); ++it)
    {
        output << "\t" << s_id << ".add_config(\"" << it.key() << "\", " << it.value() << ");" << endl;
    }
}

void add_inputs(ofstream& output, const string& s_id, const json& inputs)
{
    for(const string& key: inputs)
    {
        output << "\t" << s_id << ".add_input(\"" << key << "\");" << endl;
    }
}

void add_outputs(ofstream& output, const string& s_id, const json& outputs)
{
    for(const string& key: outputs)
    {
        output << "\t" << s_id << ".add_output(\"" << key << "\");" << endl;
    }
}


void add_setup_method(ofstream& output, const json& data)
{
    output << endl;
    output << "void setup()" << endl;
    output << "{" << endl;
    for(const json& d: data["devices"])
    {
        string id = d["id"];
        output << "\t// Device " << d["id"] << endl;
        string s_id = "s_" + id;
        output << "\tDeviceSettings " << s_id << ";" << endl;
        for(json::const_iterator it = d.begin();it!=d.end();++it)
        {
            string key = it.key();
            if(key == "config")
            {
                add_config(output, s_id, it.value());
            }
            else if(key == "inputs")
            {
                add_inputs(output, s_id, it.value());
            }
            else if(key == "outputs")
            {
                add_outputs(output, s_id, it.value());
            }
        }

        output << "\tdc.add_or_set_device(&factory, " << d["id"] << ", " << d["type"] << ", " << s_id << ");" << endl;
        
    }

    output << endl << "\tdc.setup();" << endl;

    output << "// TODO générer le code pour les expressions!!" << endl;
    int i=1;
    Expression_Parser parser;
    for(const auto& b: data["behaviors"])
    {
        string if_expr = b["if"];
        string then_expr = b["then"];
        string else_expr = b["else"];
        auto result_if = parser.parse(if_expr);
        output << "Operation_Expression* if_expr" << i << " = new " << result_if.expression->to_cstr() << "();" << endl;
        
        // A partir on a un problème avec le polymorphisme pour accéder aut types des membres potentiels.
        // Une piste serait d'utiliser le pattern visitor

        if(result_if)
        {
            output << "//TEST: " << result_if.expression->to_cstr() << endl;
        }
        output << "// Behavior : " << i << endl;
        output << "// if expression: " << if_expr << endl;
        output << "// then expression: " << then_expr << endl;
        output << "// else expression: " << else_expr << endl << endl;
        i++;
    }
    
    output << "}" << endl;
}

void add_loop_method(ofstream& output)
{
    output << endl;
    output << "void loop()" << endl;
    output << "{" << endl;
    output << "\tfor(int i=0; i< nb_behaviors;i++)" << endl;
    output << "\t{" << endl;
    output << "\t\tbehaviors[i]->process(&dc);" << endl;
    output << "\t}" << endl;
    output << "}" << endl;
}


void TestCodeBuilder::build(const json& json)
{
    ofstream output("main_code.cpp");

    add_include(output, get_factory_include());

    add_global_declarations(output, json);
    add_setup_method(output, json);
    add_loop_method(output);
}