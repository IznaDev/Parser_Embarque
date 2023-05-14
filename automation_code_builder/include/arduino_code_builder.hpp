#pragma once

/**
 * @file arduino_code_builder.hpp
 * @author Mathias Tiberghien (mathias.tiberghien@thalesgroup.com)
 * @brief Deffines Implementations for Arduino code generation
 * @version 0.1
 * @date 2023-05-14
 * 
 * @copyright © Thales Services Numériques 2023
 * 
 */

#include "code_builder.hpp"
#include <set>
#include <algorithm>
#include <iostream>
#include <string>
#include "expression.hpp"

/**
 * @brief This class is in charge for codde generation of the main Arduino application.
 * It also generates dynamically the ArduinoDataContext class, embedding the drivers used by json specification.
 * The code builder also copy the output folder the source code files needed by the main application (expressions, datacontext, ...)
 * 
 */
class ArduinoCodeBuilder : public ICodeBuilder
{

    public:
        /**
         * @brief Process the json object representing the automaton specification and generates code files in the output_directory
         * 
         * @param json The json object representing the the automaton specification
         * @param output_directory The folder path where the code will be generated
         */
        void build(const json& json, const filesystem::path& output_directory) override;
};

/**
 * @brief Implements IExpressionVisitor to be able to generate expression instantiation code.
 * This is a Visitor Design Pattern implementation
 * 
 */
class ArduinoCodebuilderExpressionVisitor: public IExpressionVisitor
{
    ostream& output;
    const string& expr_id;
    private:
        void add_member(int& index, const Expression* expr);
        void add_arg(int& index, const Expression* expr);
        void add_member_ref(const Expression* expr);
    public:
        /**
         * @brief Construct a new Arduino Codebuilder Expression Visitor object
         * 
         * @param output The outputs stream where the main code will be written
         * @param epxr_id An index that will define created variables in a unique way
         */
        ArduinoCodebuilderExpressionVisitor(ostream& output, const string& epxr_id): output(output), expr_id(epxr_id){}
        /**
         * @brief The entry method for the visitor that should call accept method on expr instance
         * 
         * @param expr A pointer to a base expression
         */
        void visit(const Expression* expr) override;
        /**
         * @brief Handles code generation for constant expressions
         * 
         * @param expr A pointer to a constant expression
         */
        void visit(const Constant_Expression* expr) override;
        /**
         * @brief Handles code generation for refeerence expressions
         * 
         * @param expr A pointer to a reference expression
         */
        void visit(const Reference_Expression* expr) override;
        /**
         * @brief Handles code generation for binary operation expressions (e.g +,-,>,==,...)
         * 
         * @param expr A pointer to a binary operation expression
         */
        void visit(const Binary_Operation_Expression* expr) override;
        /**
         * @brief Handles code generation for affectation expressions (e.g =,+=,-=)
         * 
         * @param expr A pointer to an affectation expression
         */
        void visit(const Affectation_Expression* expr) override;
        /**
         * @brief Handles code generation for unary operation expressions (e.g !)
         * 
         * @param expr A pointer to an unary operation expression
         */
        void visit(const Unary_Operation_Expression* expr) override;
        /**
         * @brief Handles code generation for function expressions (e.g min, max,...)
         * 
         * @param expr A pointer to a function expression
         */
        void visit(const Function_Expression* expr) override;
};