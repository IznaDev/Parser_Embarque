#include "parser.hpp"
#include "code_builder_factory.hpp"
#include "validator.hpp"

const string validation_schema_path = "../../schema.json";

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
    file.close();
    Automation_Validator validator(validation_schema_path);
    auto validation_result = validator.validate(json);
    if(validation_result)
    {
        filesystem::path output_directory ("generated_code");
        cout << "Generating code to: " << filesystem::absolute(output_directory) << endl;
        if(filesystem::exists(output_directory))
        {
            filesystem::remove_all(output_directory);
        }
        filesystem::create_directory(output_directory);

        ICodeBuilder* builder = CodeBuilderFactory::build(json["target"]);
        builder->build(json, output_directory);
        cout << "Completed" << endl;
    }
    else
    {
        cout << "'" << file_path << "' has an invalid schema : " << validation_result.message << endl;
    }

    return 0;
}