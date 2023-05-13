#include "validator.hpp"

Automation_Validator::Automation_Validator(const string& schema_file_path)
{
    validation_handler = make_unique<Schema_JSONValidationHandler>(schema_file_path);
    auto target_validation = make_unique<Target_JSONValidationHandler>();
    auto device_validation = make_unique<Device_JSONValidationHandler>();
    device_validation->set_next(make_unique<Expression_JSONValidationHandler>());
    target_validation->set_next(move(device_validation));
    validation_handler->set_next(move(target_validation));
}

json Schema_JSONValidationHandler::createSchema() const
{
    ifstream input(schema_file_path);
    json schemaJson = json::parse(input);
    input.close();
    return schemaJson;
}

Validation_Result Schema_JSONValidationHandler::validate(const json& file) const noexcept
{
    try
    {
        validator.validate(file);
    }
    catch(const std::exception& e)
    {
        return Validation_Result(false, e.what());
    }
    
    return this->validate_next(file);
}

Validation_Result Target_JSONValidationHandler::validate(const json& file) const noexcept
{
    string target = file["target"];
    if(supported_targets.is_supported(target))
    {
        return this->validate_next(file);
    }
    else
    {
        ostringstream s;
        s << "The target '" << target << "' is not supported by the code builder";
        return Validation_Result(false, s.str());
    }
}

Validation_Result Device_JSONValidationHandler::validate(const json& file) const noexcept
{
    string target_name = file["target"];
    const DeviceCollection& target = supported_targets.get_target(target_name);
    set<string> device_types;
    for(const auto& d: file["devices"])
    {
        device_types.emplace(d["type"]);
    }
    for(const auto& t: device_types)
    {
        if(!target.is_supported(t))
        {
            ostringstream s;
            s << "The device '" << t << "' is not supported by the code builder";
            return Validation_Result(false, s.str());
        }
    }

    return this->validate_next(file);

}

Validation_Result Expression_JSONValidationHandler::validate(const json& file) const noexcept
{
    Expression_Parser parser;
    int index=1;
    ValidatorExpressionVisitor validation_visitor(file);
    for(const auto& b: file["behaviors"])
    {
        string b_exprs[] = {b["if"], b["then"], b["else"]};
        int expr_ind=0;
        for(const auto& expr: b_exprs)
        {
            auto p_result = parser.parse(expr);
            string expr_id = expr_ind == 0 ? "if" : expr_ind == 1 ? "then" : "else";
            if(!p_result)
            {
                
                ostringstream s;
                s << "Expression '" << expr_id << "' of behavior " << index << " was not parsed correctly with the following message:" << endl;
                s << p_result.error_message << endl;
                s << "Expression text was: " << expr;
                return Validation_Result(false, s.str());
            }
            else
            {
                validation_visitor.visit(p_result.expression);
                const auto& v_result = validation_visitor.get_v_result();
                if(!v_result)
                {
                    ostringstream s;
                    s << "Expression '" << expr_id << "' of behavior " << index << " has a wrong reference:" << endl;
                    s << v_result.message << endl;
                    s << "Expression text was: " << expr;
                    return Validation_Result(false, s.str());
                }
            }
            expr_ind++;
        }
        index++;
    }

    return this->validate_next(file);

}

Validation_Result Automation_Validator::validate(const json& file) const noexcept
{
    return validation_handler->validate(file);
}

void ValidatorExpressionVisitor::visit(const Expression* expr)
{
    expr->accept(this);
}

void ValidatorExpressionVisitor::visit(const Constant_Expression* expr)
{
}

void ValidatorExpressionVisitor::visit(const Reference_Expression* expr)
{
    string ref = expr->get_ref();
    int dot_id = ref.find(".");
    string device_id = dot_id >=0 ? ref.substr(0, dot_id) : ref;
    if(device_refs.find(device_id) == device_refs.end())
    {
        this->v_result.is_valid = false;
        this->v_result.message = "Device reference '" + device_id +"' does not exist";
    }
    else
    {
        string value_id = dot_id >=0 ? ref.substr(dot_id+1) : "";
        if(!value_id.empty())
        {
            const auto& d_ref = device_refs[device_id];
            const set<string>& refs = look_for_outputs ? d_ref.outputs : d_ref.inputs;
            if(refs.find(value_id)== refs.end())
            {
                this->v_result.is_valid = false;
                this->v_result.message = "Device value reference '" + ref + "' does not exist";
            }
        }
    }
}

void ValidatorExpressionVisitor::visit(const Binary_Operation_Expression* expr)
{
    this->visit(expr->get_left_member());
    if(v_result) this->visit(expr->get_right_member());
}

void ValidatorExpressionVisitor::visit(const Affectation_Expression* expr)
{
    this->look_for_outputs = true;
    this->visit(expr->get_left_member());
    this->look_for_outputs = false;
    if(v_result) this->visit(expr->get_right_member());
}

void ValidatorExpressionVisitor::visit(const Function_Expression* expr)
{
    for(int i=0;i<expr->args_size(); i++)
    {
        this->visit(expr->get_arg(i));
        if(!v_result)
        {
            break;
        }
    }
}

void ValidatorExpressionVisitor::visit(const Unary_Operation_Expression* expr)
{
    if(v_result)
    this->visit(expr->get_right_member());
}