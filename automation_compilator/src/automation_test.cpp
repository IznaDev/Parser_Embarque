#include "automation_test.hpp"


void add_global_declarations(ostream& output, const string& target)
{
    output << endl;
    output << "// Global variables" << endl;
    output << "IDeviceFactory* factory = Automation::getFactory(\"" << target << "\");" << endl;
    output << "DeviceDataContext dc;" << endl << endl;
}

vector<string> get_device_includes(string json)
{
    vector<string> result;
    return result;
}


void TestCodeBuilder::build(string json)
{
    ofstream output("main_code.cpp");

    for(auto& include: get_core_includes())
    {
        add_include(output, include);
    }
    for(auto& include: get_device_includes(json))
    {
        add_include(output, include);
    }

    //TODO get from json
    string target = "test";

    add_global_declarations(output, target);
}