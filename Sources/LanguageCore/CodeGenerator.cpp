//
// Created by Balalarga on 09.03.2022.
//

#include "CodeGenerator.h"
#include "Functions.h"

#include <fmt/format.h>

#include <iostream>
using namespace std;

std::map<std::string, std::string> CodeGenerator::defaultOperationsMappings
{
        {"+", "{0} + {1}"},
        {"-", "{0} - {1}"},
        {"/", "{0} / {1}"},
        {"*", "{0} * {1}"},
        {"^", "pow({0}, {1})"},
        {"|", "{0} + {1} + sqrt(pow({0}, 2) + pow({1}, 2))"},
        {"&", "{0} + {1} - sqrt(pow({0}, 2) + pow({1}, 2))"},
};

CodeGenerator::CodeGenerator(std::map<std::string, std::string>&& functionsMappings,
                             std::map<std::string, std::string>&& operationsMappings):
        _functionsMapping(std::move(functionsMappings)),
        _operationsMapping(std::move(operationsMappings))
{
    _operationsMapping.merge(defaultOperationsMappings);
    CheckMappings();
}

void CodeGenerator::GetFunctionCode(const std::string& name, std::stringstream &code)
{
    code << _functionsMapping[name];
}

void CodeGenerator::GetOperationCode(const std::string& name, std::stringstream &code)
{
    code << _functionsMapping[name];
}

void CodeGenerator::CheckMappings()
{
    for (auto& func: Functions::GetAll())
    {
        if (_functionsMapping.find(func.name) == _functionsMapping.end())
            throw std::runtime_error("Couldn't find code for " + func.name + " function");
    }
}

std::string CodeGenerator::Generate(spExpression root)
{
    stringstream code;
    queue<pair<int, Expression*>> nodes;
    root->Visit(nodes);
    cout << "Visiting AST\n";
    while (!nodes.empty())
    {
        if (auto expr = dynamic_cast<NumberExpression*>(nodes.front().second))
        {
            code << expr->GetValue();
        }
        else if (auto expr = dynamic_cast<ArgumentExpression*>(nodes.front().second))
        {
            code << expr->name;
        }
        else if (auto expr = dynamic_cast<VariableExpression*>(nodes.front().second))
        {
            code << expr->name;
        }
        else if (auto expr = dynamic_cast<UnaryOperation*>(nodes.front().second))
        {
            code << expr->operation.name;
        }
        else if (auto expr = dynamic_cast<BinaryOperation*>(nodes.front().second))
        {
            expr->leftChild
        }
        else if (auto expr = dynamic_cast<FunctionExpression*>(nodes.front().second))
        {

        }
    }

    return code.str();
}

void CodeGenerator::GetExpressionCode(Expression*& expression, std::stringstream &code)
{
    if (auto expr = dynamic_cast<NumberExpression*>(expression))
    {
        code << expr->name;
    }
    else if (auto expr = dynamic_cast<ArgumentExpression*>(expression))
    {
        code << expr->name;
    }
    else if (auto expr = dynamic_cast<VariableExpression*>(expression))
    {
        code << expr->name;
    }
    else if (auto expr = dynamic_cast<UnaryOperation*>(expression))
    {
        code << expr->operation.name;
    }
    else if (auto expr = dynamic_cast<BinaryOperation*>(expression))
    {
        code << fmt::format(GetOperationCode());
    }
    else if (auto expr = dynamic_cast<FunctionExpression*>(expression))
    {
        code << expr->function.name;
    }
}
