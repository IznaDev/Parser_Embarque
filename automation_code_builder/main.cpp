#include "parser.hpp"
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
    file.close();
    //3. Générer le code qui décrit les settings
    // TODO oussama: à compléter et tester
    filesystem::path output_directory ("generated_code");
    cout << "Generating code to: " << filesystem::absolute(output_directory) << endl;
    if(!filesystem::exists(output_directory))
    {
        filesystem::create_directory(output_directory);
    }

    ArduinoCodeBuilder builder;
    builder.build(json, output_directory);
    cout << "Completed" << endl;

    return 0;
}