//
// Created by Balalarga on 06.03.2022.
//

#include "Program.h"

double Program::Process()
{
    return _begin->GetValue();
}

void Program::Init(spExpression begin)
{
    _begin = std::move(begin);
    std::queue<spExpression> expressions;
    _begin->Visit(expressions);
    while(!expressions.empty())
    {
        auto &expr = expressions.front();
        expressions.pop();

        if (auto var = std::dynamic_pointer_cast<VariableExpression>(expr))
            _variables.push_back(var);
        else if (auto arg = std::dynamic_pointer_cast<ArgumentExpression>(expr))
            _arguments.push_back(arg);
    }
}
