#include "test_code_builder.hpp"

#include <iostream>
using namespace std;

void add_global_declarations(ostream& output)
{
    output << endl;
    output << "// Global variables" << endl;
    output << "TestFactory factory;" << endl;
    output << "DeviceDataContext dc;" << endl << endl;
}

string get_factory_include()
{
    return "automation_test.hpp";
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

        output << "\tdc.add_or_set_device(&factory, id, type, " << s_id << ");" << endl;
        
    }

    output << endl << "\tdc.setup();" << endl;
    
    output << "}" << endl;
}


void TestCodeBuilder::build(const json& json)
{
    ofstream output("main_code.cpp");

    for(auto& include: get_core_includes())
    {
        add_include(output, include);
    }
    add_include(output, get_factory_include());

    add_global_declarations(output);
    add_setup_method(output, json);
}