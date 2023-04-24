#include "parser.hpp"

Expression_Parser::Expression_Parser()
{
    operation_factory.insert({"+", [](){return make_unique<Addition_Expression>();}});
    operation_factory.insert({"-", [](){return make_unique<Substraction_Expression>();}});
    operation_factory.insert({"*", [](){return make_unique<Multiplication_Expression>();}});
    operation_factory.insert({"/", [](){return make_unique<Division_Expression>();}});
    operation_factory.insert({"%", [](){return make_unique<Modulo_Expression>();}});
    operation_factory.insert({"==", [](){return make_unique<Equals_Expression>();}});
    operation_factory.insert({"<", [](){return make_unique<LessThan_Expression>();}});
    operation_factory.insert({"<=", [](){return make_unique<LessThanEqual_Expression>();}});
    operation_factory.insert({">", [](){return make_unique<GreaterThan_Expression>();}});
    operation_factory.insert({">=", [](){return make_unique<GreaterThanEqual_Expression>();}});
    operation_factory.insert({"!=", [](){return make_unique<NotEquals_Expression>();}});
    operation_factory.insert({"and", [](){return make_unique<And_Expression>();}});
    operation_factory.insert({"or", [](){return make_unique<Or_Expression>();}});
    operation_factory.insert({"!", [](){return make_unique<Not_Expression>();}});
    operation_factory.insert({"^", [](){return make_unique<XOR_Bitwise_Expression>();}});
    operation_factory.insert({"&", [](){return make_unique<And_Bitwise_Expression>();}});
    operation_factory.insert({"|", [](){return make_unique<Or_Bitwise_Expression>();}});
    operation_factory.insert({"<<", [](){return make_unique<Shift_Left_Expression>();}});
    operation_factory.insert({">>", [](){return make_unique<Shift_Right_Expression>();}});
    operation_factory.insert({"=", [](){return make_unique<Set_Expression>();}});
    operation_factory.insert({"+=", [](){return make_unique<Increase_Expression>();}});
    operation_factory.insert({"-=", [](){return make_unique<Decrease_Expression>();}});

    function_factory.insert({"max", [](){return make_unique<Max_Function_Expression>();}});
    function_factory.insert({"min", [](){return make_unique<Min_Function_Expression>();}});
    function_factory.insert({"pow", [](){return make_unique<Pow_Function_Expression>();}});
    function_factory.insert({"sqrt", [](){return make_unique<Sqrt_Function_Expression>();}});

    validator = make_unique<EmptyValidationHandler>();
    auto v2 = make_unique<ParenthesisValidationHandler>();
    auto v3 = make_unique<ExpressionValidationHandler>(*this);
    auto v4 = make_unique<AffecationValidationHandler>(*this);
    v3->set_next(move(v4));
    v2->set_next(move(v3));
    validator->set_next(move(v2));
    
}

void add_member(unique_ptr<Operation_Expression>& expr, unique_ptr<Expression>& member, bool is_left_member)
{
    if(expr->can_add_member() && (!is_left_member || expr->has_left_member()))
    {
        expr->add_member(move(member));
    }
    else
    {
        member = nullptr;
    }
}

string Expression_Parser::get_ops_regex_string(const string& expr) const
{
    vector<string> ops = getSupportedOperatorsReg();
    sort(begin(ops), end(ops), [](const string& s1, const string& s2){return s1.size()>s2.size();});
    stringstream s_ops;
    s_ops << "(\\s|\\(|\\)";
    if(ops.size()>0)
    {
        s_ops << "|";
    }
    for(int i=0;i<ops.size();i++)
    {
        s_ops << ops[i];
        if(i<ops.size()-1)
        {
            s_ops << "|";
        }
    }
    s_ops << ")";

    return s_ops.str();

}

string Expression_Parser::get_func_regex_string(const string& expr) const
{
    vector<string> funcs = getSupportedFunctions();
    sort(begin(funcs), end(funcs), [](const string& s1, const string& s2){return s1.size()>s2.size();});
    stringstream s_func;
    s_func << "(";
    for(int i=0;i<funcs.size();i++)
    {
        s_func << funcs[i];
        if(i<funcs.size()-1)
        {
            s_func << "|";
        }
    }
    s_func << ")\\(";

    return s_func.str();

}

Validation_Result ExpressionValidationHandler::validate(const string& expr) const noexcept
{
    string r_ops = parser.get_ops_regex_string(expr);
    string r_func = parser.get_func_regex_string(expr);
    string expr_without_funcs = regex_replace(expr, regex(r_func, regex_constants::icase), "");
    expr_without_funcs = regex_replace(expr_without_funcs, regex(","), " + ");
    string digits = regex_replace(expr_without_funcs, regex(r_ops, regex_constants::icase), "");
    regex is_ref("\\$\\{\\w+(\\.\\w+){0,}\\}");
    digits = regex_replace(digits, is_ref, "1");
    auto invalid_it = find_if_not(begin(digits), end(digits), [](const char c){return isdigit(c);});
    if(invalid_it != digits.end())
    {
        ostringstream message;
        message << "The following operator is not supported:  '" << *invalid_it << "'";
        return Validation_Result{false,message.str()};
    }

    // Expression should be separated by operators
    stringstream splitters;
    splitters << "[\\s()]+";
    regex re(splitters.str());
    sregex_token_iterator first{expr_without_funcs.begin(), expr_without_funcs.end(), re, -1}, last;
    vector<std::string> tokens{first, last};
    tokens.erase(remove_if(begin(tokens),end(tokens), is_whitespace_or_empty), end(tokens));
    if(tokens.size()>1)
    {
        regex end_with_operation("^[\\d" + r_ops+ "]*"+r_ops+"$", regex_constants::icase);
        regex start_with_operation("^" + r_ops+"[\\d" + r_ops+ "]*$", regex_constants::icase);
        bool check_operator = false;
        for(auto& word: tokens)
        {
            word = regex_replace(word, is_ref, "1");
            if(check_operator && !regex_match(word, start_with_operation))
            {
                return Validation_Result{false,"Two expressions should be separated by an operator"};
            }
            check_operator = !regex_match(word, end_with_operation);
        }
        
    }

    return validate_next(expr);
}

Validation_Result AffecationValidationHandler::validate(const string& expr) const noexcept
{
    vector<string> supported_ops = parser.getSupportedOperators();
    sort(supported_ops.begin(), supported_ops.end(), greater());
    ostringstream s_updatable;
    ostringstream s_others;
    s_updatable << "(";
    s_others << "(";
    int i_updatable = 0;
    int i_others = 0;
    for(const string& op_id: supported_ops)
    {
        auto op = parser.operation_factory.at(op_id)();
        if(op->is_updatable())
        {
            if(i_updatable++>0)
            {
                s_updatable << "|";
            }
            s_updatable << op->get_reg_id();
        }
        else
        {
            if(i_others++>0)
            {
                s_others <<"|";
            }
            s_others << op->get_reg_id();
        }
    }
    s_updatable << ")";
    s_others << ")";
    string r_up_string = s_updatable.str();
    string r_ot_string = s_others.str();
    string expr_bis = regex_replace(expr, regex(r_ot_string), "");
    regex r_ops(r_up_string);
    int nb_affectation_ops = distance(sregex_iterator(expr_bis.begin(), expr_bis.end(), r_ops),sregex_iterator());
    if(nb_affectation_ops >= 1)
    {
        if(nb_affectation_ops > 1)
        {
            return Validation_Result{false, "Only one affectation operator is allowed"};
        }
        regex is_ref("(\\$\\{\\w+(\\.\\w+){0,}\\})");
        expr_bis = regex_replace(expr_bis, is_ref, "");
        regex is_left_member_ref("^\\s*"+r_up_string+".*$");
        if(!regex_match(expr_bis, is_left_member_ref))
        {
            return Validation_Result{false, "Affectation left member should be a reference"};
        };
    }

    return validate_next(expr);
}

Validation_Result Expression_Parser::validate(const string& expr) const noexcept
{
    return validator->validate(expr);
}

vector<string> Expression_Parser::getSupportedOperators() const
{
    vector<string> result;
    for(const auto&p: operation_factory)
    {
        result.push_back(p.first);
    }
    return result;
}

vector<string> Expression_Parser::getSupportedFunctions() const
{
    vector<string> result;
    for(const auto&p: function_factory)
    {
        result.push_back(p.first);
    }
    return result;
}

vector<string> Expression_Parser::getSupportedOperatorsReg() const
{
    vector<string> result;
    for(const auto&p: operation_factory)
    {
        result.push_back(p.second()->get_reg_id());
    }
    return result;
}

void unstack_operations(stack<unique_ptr<Operation_Expression>>& ops)
{
    unique_ptr<Expression> member{nullptr};
    while(ops.size()>1)
    {
        member = move(ops.top());
        ops.pop();
        add_member(ops.top(), member, false);
    }
}

char safe_to_lower(char c)
{
    return static_cast<char>(tolower(static_cast<unsigned char>(c)));
}

bool Expression_Parser::is_id(vector<string>& supported_ids, istringstream& stream, string& id) const
{
    vector<string> match_ids;
    copy_if(begin(supported_ids), end(supported_ids), back_insert_iterator(match_ids), [&id](const string& known_id){return safe_to_lower(known_id[0])==safe_to_lower(id[0]);});
    //No match means not an operator
    if(match_ids.size()==0)
    {
        return false;
    }
    // If only one match and size is one return true othewise check if following chars are matching with the id
    if(match_ids.size() == 1)
    {
        int id_size = match_ids[0].size();
        if(id_size == 1)
        {
            return true;
        }
        else
        {
            stringstream ids;
            ids << safe_to_lower(id[0]);
            int i=1;
            for(i;i<id_size;i++)
            {
                char c;
                stream.get(c);
                ids << safe_to_lower(c);
            }
            string complete_id = ids.str();
            if(complete_id == match_ids[0])
            {
                id = complete_id;
                return true;
            }
            else
            {
                stream.seekg(-(i-1), ios_base::cur);
                return false;
            }
        }
    }
    // If more than one match we should find the unique match looking to following characters
    int offset=1;
    stringstream ids;
    ids << id;
    do
    {
        char c;
        stream.get(c);
        ids << c;
        offset++;
        string match_id;
        int n = count_if(begin(match_ids), end(match_ids), [&match_id, &ids, offset](const string& s){ 
            bool result = s.substr(0,offset) == ids.str();
            if(result)
            {
                match_id = s;
            }
            return result;
        });
        if(n >= 1)
        {
            if(n==1)
            {
                id = match_id;
                while(offset< match_id.size())
                {
                    stream.get(c);
                    offset++;
                }
                return true;
            }
            else
            {
                id = ids.str();
            }
           
        }
        else
        {
            stream.seekg(-(offset-1), ios_base::cur);
            return true;
        }

    } while (true);
    
    return false;
}

bool is_reference_id(istringstream& s, string& id)
{
    if(id[0]=='$')
    {
        ostringstream ref;
        char c;
        s.get(c);
        do
        {
            s.get(c);
            if(c!='}')
            {
                ref << c;
            }

        } while (c!='}');
        id = ref.str();
        return true;
    }
    return false;

}

Parse_Result Expression_Parser::build_function(const string& id, istringstream& s) const noexcept
{
    char c;
    // First parenthesis;
    int opening_count = 0;
    int read_chars = 0;
    int nb_args =0;
    do
    {
        s.get(c);
        read_chars++;
        if(isspace(c))
        {
            continue;
        }
        if(c=='(')
        {
            opening_count++;
            if(opening_count==1)
            {
                read_chars=0;
            }
        }
        else if(c==')')
        {
            opening_count--;
        }
        else if(opening_count == 1 && c==',')
        {
            nb_args++;
        }
        else if(nb_args==0)
        {
            nb_args++;
        }
    } while (opening_count>0 && !s.eof());
    s.seekg(-(read_chars), ios_base::cur);
    
    Parse_Result result;
    auto function = function_factory.at(id)();
    while(nb_args>0)
    {
        auto p = parse(s);
        if(p)
        {
            function->add_arg(move(p.expression));
            nb_args--;
        }
        else
        {
            result.error_message = p.error_message;
            return result;
        }
        
    }
    result.expression = move(function);
    return result;
}

Parse_Result Expression_Parser::parse(istringstream& s) const noexcept
{
    Parse_Result result;
    ostringstream word{""s};
    unique_ptr<Operation_Expression> current{nullptr};
    unique_ptr<Expression> member{nullptr};
    stack<unique_ptr<Operation_Expression>> ops;
    vector<string> supportedOperators = getSupportedOperators();
    vector<string> supportedFunctions = getSupportedFunctions();
    char c;
    try
    {
        while(s.get(c))
        {
            if(isspace(c)) continue;
            // Return if end of group of end of function argument
            if(c==')' || c==',')break;
            string id{c};
            // Parse recursively if start of group
            if(c=='(')
            {
                auto p = parse(s);
                // If conversion suceed store as a member
                if(p)
                {
                    member = move(p.expression);
                }
                // Else return error message
                else
                {
                    result.error_message = p.error_message;
                    return result;
                }
                // When a group is parsed we just need to read the next character
                continue;
            }
            else if(is_reference_id(s, id))
            {
                member = make_unique<Reference_Expression>(id);
                continue;
            }
            else if(is_id(supportedFunctions, s, id))
            {
                auto p = build_function(id, s);
                if(p)
                {
                    member = move(p.expression);
                    continue;
                }
                else
                {
                    result.error_message = p.error_message;
                    return result;
                }
            }
            // Check if the character is an operator or the first character of an operator
            // Note that the id will contain the full id of the operation if succeed
            if(is_id(supportedOperators, s, id))
            {
                // If member is not defined make member as constant expression
                string constant = word.str();
                if(!member)
                {
                    member = make_unique<Constant_Expression>(constant);
                }
                // Word is cleared
                word.str("");
                //Create the operation expression according to the id
                current = build_operation(id);
                //If there is another operation in the stack
                if(!ops.empty())
                {
                    //If new operator has a greater priority than previous
                    if(*ops.top() < *current)
                    {
                        // Member is added to the current operator
                        add_member(current, member, true);
                    }
                    else
                    {
                        // Member is added  to previous operator
                        add_member(ops.top(), member, false);
                        // If new operator has lower priority previous members can be merged
                        if(*current < *ops.top())
                        {
                            //All operations are merged from right to left
                            //There oldest operation will remain in the stack
                            unstack_operations(ops);
                        }
                        // There is one (after unstack) or more operations
                        member = move(ops.top());
                        // Previous operation is merged to the current
                        add_member(current, member, true);
                        ops.pop(); 
                    }
                }
                else
                {
                    // Member is added to current operation
                    add_member(current, member, true);
                }
                // Current operation is added to the stack
                ops.push(move(current));
            }
            else
            {
                // If c is not an operato the word stream accumulates it
                word << c;
            }
        }
        // At the end of the stream one last member shoud remain.
        string constant = word.str();
        if(!member)
        {
            member = make_unique<Constant_Expression>(constant);
        }
    }
    catch(const std::exception& e)
    {
        result.error_message = e.what();
        return result;
    }
    
    // If the stack of operation is not empty, remainins operations should be merged from right to left
    if(!ops.empty())
    {
        //Last member is added to last operation
        add_member(ops.top(), member, false);
        // Previous operations are merged   
        unstack_operations(ops);
        // The result contains the top operation
        result.expression = move(ops.top());
        ops.pop();
    }
    else
    {
        //In case no operators the expression is a group or a constant
        result.expression = move(member);
    }
    return result;
}

Validation_Result Expression_Parser::add_customFunction(const string& id, const string& expression)
{
    auto p = this->parse(expression);
    if(p)
    {
        function_factory.insert({id, [id, expression,this](){
            auto p = this->parse(expression);
            return make_unique<Custom_Function_Expression>(id, move(p.expression));}});
        return Validation_Result();
    }


    return Validation_Result{false, p.error_message};
}

Parse_Result Expression_Parser::parse(const string& expression_string) const noexcept
{
    Validation_Result validation = validate(expression_string);
    Parse_Result result;
    if(validation)
    {
        istringstream s{expression_string};
        return parse(s);
    }
    else
    {
        result.error_message = validation.message;
    }
    return result;
}