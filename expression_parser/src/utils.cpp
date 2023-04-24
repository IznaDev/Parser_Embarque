#include "utils.hpp"

bool is_whitespace_or_empty(const string& text)
{
    return text.empty() || text.find_first_not_of(" \n\t") == string::npos;
}