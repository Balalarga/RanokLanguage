//
// Created by Balalarga on 06.03.2022.
//

#include "Program.h"

double Program::Process()
{
    return _root->GetValue();
}

void Program::Init(const spExpression& root)
{
    _root = root;
}

spFunctionExpression Program::ToFunctionExpression()
{
    return spFunctionExpression();
}
