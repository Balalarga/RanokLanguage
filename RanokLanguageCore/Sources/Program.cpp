//
// Created by Balalarga on 06.03.2022.
//

#include "Program.h"

double Program::Process()
{
    double value = _root->GetValue();
//    std::cout << "Before reset\n";
//    Dump();
    _root->Reset();
//    std::cout << "After reset\n";
//    Dump();
    return value;
}

void Program::Init(const spExpression& root)
{
    _root = root;
}
