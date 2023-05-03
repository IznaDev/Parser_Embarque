#include "parser.hpp"

using namespace std;

int main()
{
    Expression_Parser parser;
    cout << "Mathematical expression parser" << endl;
    cout << "Supported operators: ";
    vector<string> ops = parser.getSupportedOperators();
    for_each(begin(ops), end(ops), [i=0,size=ops.size()](const string& s) mutable {cout << s << (++i < size ? ", " : "\n");});
    cout << "Supported functions: ";
    vector<string> funcs = parser.getSupportedFunctions();
    for_each(begin(funcs), end(funcs), [i=0,size=funcs.size()](const string& s) mutable {cout << s << (++i < size ? ", " : "\n");});
    cout << "Group separators: ()" << endl;
    string expression;
    vector<pair<string,string>> custom_functions{{"is_even","${1}%2==0"},{"is_odd","!is_even(${1})"},{"sum", "${1} + ${2}"},{"add_var_1", "${1} + ${var_1}"}};
    cout << "Custom Functions:" << endl;
    for(const auto&p: custom_functions)
    {
        auto result = parser.add_customFunction(p.first, p.second);
        if(result)
        {
            cout << p.first << ": " << p.second << endl;
        }
        else
        {
            cout << "Could not add function '" << p.first << "' : " << result.message << endl;
        }
    }
    
    MemoryDataContext dc;
    dc.add_or_set("var_1", 5);
    dc.add_or_set("var_2", 3);
    cout << "List of references:" << endl;
    for(int i=0;i<dc.get_size();i++)
    {
        cout << dc.get_key(i) << ": " << dc.evaluate(dc.get_key(i)) << endl;
    }
    while(expression!="q")
    {
        cout << "Your expression ('q' to quit): ";
        getline(cin, expression);
        if(expression!="q")
        {
            auto parse_result = parser.parse(expression);
            if(parse_result)
            {
                long result = parse_result.expression->evaluate(&dc);
                if(parse_result.expression->is_updatable())
                {
                    parse_result.expression->update(&dc);
                }
                cout << parse_result.expression->to_string() << endl;
                cout << expression << " = " << result << endl;
            }
            else
            {
                cout << "Invalid expression: " << parse_result.error_message << endl;
            }
        }
    }
}