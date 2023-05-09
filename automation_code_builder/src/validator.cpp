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


void validJson(string schema, json file)
{
    json_validator validator = validSchema(schema);

    if(validator.validate(file) == nullptr)
    {
        cout << "Le fichier est conforme au schéma !" << endl;
    }
    else
    {
        throw invalid_argument("Le fichier n'est pas conforme au schéma !");
    }
}