#include <iostream>
#include "../include/validator.hpp"

#define SCHEMA_PATH "../schema/schema.json"

using namespace std;

int main(int argc, char* argv[])
{
    if(argc > 2)
    {
        cout << "Trop d'arguments ! " << endl;
        return EXIT_FAILURE;
    }

    string filepath = argv[1];

    isExist(filepath);
    isCorrect(filepath);
    validJson(SCHEMA_PATH, filepath);

    return 0;
}