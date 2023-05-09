#include "code_builder.hpp"

void add_include(ostream& output, const string& include)
{
    output << "#include \"" << include << "\"" << endl;
}