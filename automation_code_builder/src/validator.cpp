#include "../include/validator.hpp"

bool isExist(string filepath)
{
    fstream file(filepath);

    if(file.good())
    {
        cout << "Le fichier " << filepath << " existe !" << endl;
        file.close();
        return true;
    }
    else
    {
        cout << "Fichier introuvable !" << endl;
        return false;
    }
}

bool isCorrect(string filepath)
{
    ifstream input(filepath);
    json j = json::parse(input);


    if(j.is_structured())
    {
        cout << "Le fichier est bien conforme au format JSON !" << endl;
        input.close();
        return true;
    }
    else
    {
        cout << "Le fichier n'est pas conforme au format JSON ou est vide!" << endl;
        input.close();
        return false;
    }
}

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

void validJson(string schema, string filepath)
{
    ifstream input(filepath);
    json compare = json::parse(input);
    json_validator validator = validSchema(schema);
    input.close();

    if(validator.validate(compare) == nullptr)
    {
        cout << "Le fichier est conforme au schéma !" << endl;
    }
    else
    {
        throw invalid_argument("Le fichier n'est pas conforme au schéma !");
    }
}