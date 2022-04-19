//
// Created by Balalarga on 07.03.2022.
//

#include "SymbolsTable.h"

std::map<std::string, double> SymbolsTable::GlobalConstants
{
    {"PI", 3.141592}
};

SymbolsTable::SymbolsTable()
{
    for (auto& i: GlobalConstants)
        _variables.push_back(std::make_shared<VariableExpression>(i.first, std::make_shared<NumberExpression>(i.second)));
}

spVariableExpression SymbolsTable::CreateArgument(const std::string& name)
{
    _arguments.push_back(std::make_shared<VariableExpression>(name, std::make_shared<NumberExpression>(0)));
    _argumentRanges.push_back({{}});
    return _arguments.back();
}

spVariableExpression SymbolsTable::CreateArgument(const std::string &name, Range range)
{
    _arguments.push_back(std::make_shared<VariableExpression>(name, std::make_shared<NumberExpression>(0)));
    _argumentRanges.push_back({range});
    return _arguments.back();
}

spVariableExpression SymbolsTable::CreateArgument(const std::string& name, std::vector<Range> ranges)
{
    _arguments.push_back(std::make_shared<VariableExpression>(name, std::make_shared<ArrayExpression>(std::vector<spExpression>(ranges.size(), std::make_shared<NumberExpression>(0)))));
    _argumentRanges.push_back(ranges);
    return _arguments.back();
}

spVariableExpression SymbolsTable::CreateVariable(const std::string& name, spExpression child)
{
    _variables.push_back(std::make_shared<VariableExpression>(name, child));
    return _variables.back();
}

spNumberExpression SymbolsTable::CreateConstant(double value)
{
    _constants.push_back(std::make_shared<NumberExpression>(value));
    return _constants.back();
}

spVariableExpression SymbolsTable::FindArgument(const std::string& name)
{
    for (auto& var: _arguments)
        if (var->name == name)
            return var;

    return nullptr;
}

spVariableExpression SymbolsTable::FindVariable(const std::string& name)
{
    for (auto& var: _variables)
        if (var->name == name)
            return var;

    return nullptr;
}

spNumberExpression SymbolsTable::FindConstant(const std::string& name)
{
    for (auto& var: _constants)
        if (var->name == name)
            return var;

    return nullptr;
}
