#include "arduino_code_builder.hpp"
#include "parser.hpp"

#include <iostream>
using namespace std;

void ArduinoCodebuilderExpressionVisitor::visit(const Expression* expr)
{
    expr->accept(this);
}

void ArduinoCodebuilderExpressionVisitor::add_member(int& index, const Expression* expr)
{
    if(expr)
    {
        if(expr->is_leaf())
        {
            output << "\t" << expr_id << "->add_member( new ";
            expr->accept(this);
            output << " );" << endl; 
        }
        else
        {
            // TODO à tester quand les membres sont des expressions ou des fonctions
            string member_id = expr_id+"_op" + to_string(index++);
            //On revisite récursivement les membres qui sont des expression ou des fonctions avant de les ajouter au parent
            ArduinoCodebuilderExpressionVisitor visitor(output, member_id);
            visitor.visit(expr);
            output << "\t" << expr_id << "->add_member( " << member_id << " );" << endl; 
        }
    }
}

void ArduinoCodebuilderExpressionVisitor::add_arg(int& index, const Expression* expr)
{
    if(expr)
    {
        if(expr->is_leaf())
        {
            output << "\t" << expr_id << "->add_arg( new ";
            expr->accept(this);
            output << " );" << endl; 
        }
        else
        {
            // TODO à tester quand les membres sont des expressions ou des fonctions
            string arg_id = expr_id+"_arg" + to_string(index++);
            //On revisite récursivement les membres qui sont des expression ou des fonctions avant de les ajouter au parent
            ArduinoCodebuilderExpressionVisitor visitor(output, arg_id);
            visitor.visit(expr);
            output << "\t" << expr_id << "->add_arg( " << arg_id << " );" << endl; 
        }
    }
}

void ArduinoCodebuilderExpressionVisitor::add_member_ref(const Expression* expr)
{
    if(expr)
    {
        output << "\t" << expr_id << "->add_ref_member( new ";
            expr->accept(this);
            output << " );" << endl; 
    }
}

void ArduinoCodebuilderExpressionVisitor::visit(const Constant_Expression* expr)
{
   output << expr->to_cstr() << "(\"" << expr->get_value() << "\")"; 
}

void ArduinoCodebuilderExpressionVisitor::visit(const Reference_Expression* expr)
{
    output << expr->to_cstr() << "(\"" << expr->get_ref() << "\")";
}

void ArduinoCodebuilderExpressionVisitor::visit(const Binary_Operation_Expression* expr)
{
    output << "\tOperation_Expression* " << expr_id << " = new " << expr->to_cstr() << "();" << endl;
    int op_index=1;
    add_member(op_index, expr->get_left_member());
    add_member(op_index, expr->get_right_member());
}

void ArduinoCodebuilderExpressionVisitor::visit(const Affectation_Expression* expr)
{
    output << "\tAffectation_Expression* " << expr_id << " = new " << expr->to_cstr() << "();" << endl;
    int op_index=1;
    add_member_ref(expr->get_left_member());
    add_member(op_index, expr->get_right_member());
}

void ArduinoCodebuilderExpressionVisitor::visit(const Function_Expression* expr)
{
    output << "\tFunction_Expression* " << expr_id << " = new " << expr->to_cstr() << "();" << endl;
    int arg_index=1;
    for(int i=0;i<expr->args_size(); i++)
    {
        add_arg(arg_index, expr->get_arg(i));
    }
}

void ArduinoCodebuilderExpressionVisitor::visit(const Unary_Operation_Expression* expr)
{
    output << "\tOperation_Expression* " << expr_id << " = new " << expr->to_cstr() << "();" << endl;
    int op_index=1;
    add_member(op_index, expr->get_right_member());
}

void add_global_declarations(ostream& output, const json& data)
{
    output << endl;
    output << "// Global variables" << endl;
    output << "ArduinoFactory factory;" << endl;
    output << "DeviceDataContext dc;" << endl;
    json b = data["behaviors"];
    int nb_behaviors = b.size();
    output << "const int nb_behaviors = " << nb_behaviors << ";" << endl;
    output << "Behavior* behaviors[nb_behaviors];" << endl;
    output << endl;
}

string get_factory_include()
{
    return "arduino_context.hpp";
}

void add_config(ofstream& output, const string& s_id, const json& config)
{
    for(json::const_iterator it = config.begin(); it!=config.end(); ++it)
    {
        output << "\t" << s_id << ".add_config(\"" << it.key() << "\", " << it.value() << ");" << endl;
    }
}

void add_inputs(ofstream& output, const string& s_id, const json& inputs)
{
    for(const string& key: inputs)
    {
        output << "\t" << s_id << ".add_input(\"" << key << "\");" << endl;
    }
}

void add_outputs(ofstream& output, const string& s_id, const json& outputs)
{
    for(const string& key: outputs)
    {
        output << "\t" << s_id << ".add_output(\"" << key << "\");" << endl;
    }
}


void add_setup_method(ofstream& output, const json& data)
{
    output << endl;
    output << "void setup()" << endl;
    output << "{" << endl;
    for(const json& d: data["devices"])
    {
        string id = d["id"];
        output << "\t// Device " << d["id"] << endl;
        string s_id = "s_" + id;
        output << "\tDeviceSettings " << s_id << ";" << endl;
        for(json::const_iterator it = d.begin();it!=d.end();++it)
        {
            string key = it.key();
            if(key == "config")
            {
                add_config(output, s_id, it.value());
            }
            else if(key == "inputs")
            {
                add_inputs(output, s_id, it.value());
            }
            else if(key == "outputs")
            {
                add_outputs(output, s_id, it.value());
            }
        }

        output << "\tdc.add_or_set_device(&factory, " << d["id"] << ", " << d["type"] << ", " << s_id << ");" << endl;
        
    }

    output << endl << "\tdc.setup();" << endl << endl;

    int i=1;
    Expression_Parser parser;
    for(const auto& b: data["behaviors"])
    {
        output << "\t// Behavior " << i << endl;
        string if_expr = b["if"];
        string then_expr = b["then"];
        string else_expr = b["else"];
        // C'est important d'utiliser la simplification d'expression pour diminuer le nombre d'instanciationss
        auto result_if = parser.parse(if_expr).expression->simplify();
        auto result_then = parser.parse(then_expr).expression->simplify();
        auto result_else = parser.parse(else_expr).expression->simplify();

        string if_expr_id = "if_expr" + to_string(i);
        string then_expr_id = "then_expr"+ to_string(i);
        string else_expr_id = "else_expr" + to_string(i);
        ArduinoCodebuilderExpressionVisitor visitor_if(output, if_expr_id);
        ArduinoCodebuilderExpressionVisitor visitor_then(output, then_expr_id);
        ArduinoCodebuilderExpressionVisitor visitor_else(output, else_expr_id);
        visitor_if.visit(result_if);
        visitor_then.visit(result_then);
        visitor_else.visit(result_else);
        output << "\tbehaviors["<< i-1 <<"] = new Behavior( " << if_expr_id << ", " << then_expr_id << ", " << else_expr_id << ");" << endl;
        i++;
    }
    
    output << "}" << endl;
}

void add_loop_method(ofstream& output)
{
    output << endl;
    output << "void loop()" << endl;
    output << "{" << endl;
    output << "\tfor(int i=0; i< nb_behaviors;i++)" << endl;
    output << "\t{" << endl;
    output << "\t\tbehaviors[i]->process(&dc);" << endl;
    output << "\t}" << endl;
    output << "}" << endl;
}

void copy_included_files(const json& json, const filesystem::path& output_directory)
{
    vector<string> expression_parser_files{
        "container.hpp",
        "data_context.hpp",
        "expression.hpp",
        "expr_mathematical.hpp",
        "expr_logical.hpp",
        "expr_functions.hpp",
        "expr_affectation.hpp"
    };

    vector<string> automation_code_builder_files
    {
        "automation.hpp",
        //TODO arduino_context.hpp doit être généré dynamiquement pour que le factory ne contienne que des rérérences
        // aux devices utilisés (pour avoir le fichier compilé le plus petit possible)
        "arduino_context.hpp",
        // TODO La liste des fichiers à inclure doit dépendre des devices du fichier json
        "hcsr501.hpp",
        "LED.hpp"
    };

    for(const auto& file: expression_parser_files)
    {
       filesystem::path src = filesystem::path("../../expression_parser/include")/filesystem::path(file);
       filesystem::path dest = output_directory/src.filename();
       filesystem::copy(src, dest, filesystem::copy_options::overwrite_existing);
    }

    for(const auto& file: automation_code_builder_files)
    {
       filesystem::path src = filesystem::path("../../automation_code_builder/include")/filesystem::path(file);
       filesystem::path dest = output_directory/src.filename();
       filesystem::copy(src, dest, filesystem::copy_options::overwrite_existing);
    }
}


void ArduinoCodeBuilder::build(const json& json, const filesystem::path& output_directory)
{

    copy_included_files(json, output_directory);
    ofstream output(output_directory / filesystem::path("main_code.cpp"));

    add_include(output, get_factory_include());

    add_global_declarations(output, json);
    add_setup_method(output, json);
    add_loop_method(output);
}