#include "validator.hpp"

json createSchema(string schema)
{
    ifstream input(schema);
    json schemaJson = json::parse(input);
    input.close();
    return schemaJson;
}

json_validator validSchema(string schema)
{
    json_validator validator;
    validator.set_root_schema(createSchema(schema));
    return validator;
}


Validation_Json validJson(string schema, json file)
{
    json_validator validator = validSchema(schema);


    if(validator.validate(file) == nullptr)
    {
        ostringstream message;
        message << "Le fichier est conforme au schéma !";
        return Validation_Json{true, message.str()};
    }
    else
    {
        ostringstream message;
        message << "Le fichier n'est pas conforme au schéma !";
        return Validation_Json{false, message.str()};
    }
}

Validation_Json TargetExists::validate_item(const json& item) const noexcept
{
    if(item["target"] != "arduino")
    {
        ostringstream message;
        message << "La cible " << item["target"] << " n'est pas supportée !";
        return Validation_Json{false, message.str()};
    }
    return validate_next_item(item);
}

Validation_Json DevicesExist::validate_item(const json& item) const noexcept
{
    if(item["device"]["type"] != "HCSR501" || item["device"]["type"] != "LED")
    {
        ostringstream message;
        message << "L'élément " << item["device"]["type"] << " n'est pas supportée !";
        return Validation_Json{false, message.str()};
    }
    return validate_next_item(item);
}

Validation_Json BehaviorsExist::validate_item(const json& item) const noexcept
{
    
    vector<string> devicesID;
    string delimiter1 = "{";
    string delimiter2 = "}";
    string delimiter3 = ".";

    for(int i = 0; i < item["devices"].size();i++)
    {
        cout << item["devices"][i]["id"] << endl;
        if(item["devices"][i]["inputs"] == true)
        {
            cout << item["devices"][i]["inputs"] << endl;
        }
        else if(item["devices"][i]["outputs"] == true)
        {
            cout << item["devices"][i]["outputs"] << endl;
        }
    }


    return validate_next_item(item);
}